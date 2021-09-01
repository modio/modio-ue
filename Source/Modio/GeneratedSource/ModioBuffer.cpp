/* 
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *  
 *  This file is part of the mod.io SDK.
 *  
 *  Distributed under the MIT License. (See accompanying file LICENSE or 
 *   view online at <https://github.com/modio/modio-sdk/blob/main/LICENSE>)
 *  
 */

#ifdef MODIO_SEPARATE_COMPILATION
	#include "modio/core/ModioBuffer.h"
#endif

#include <limits>

namespace Modio
{
	namespace Detail
	{
		std::size_t Buffer::GetAlignment() const
		{
			return Alignment;
		}

		Buffer::Buffer(std::size_t Size, std::size_t Alignment /*= 1*/) : Alignment(Alignment), Size(Size)
		{
			// find how many 4k blocks we need to allocate
			auto NumBlocks = ((Size / (4 * 1024)) + 1);
			// Add worst case amount of alignment bytes
			auto HeaderSize = Alignment - 1;
			// Calculate size big enough to include the desired size (padded to 4k boundaries) + the worst-case
			// amount of alignment bytes
			auto TotalSize = NumBlocks * (4 * 1024) + HeaderSize;

			InternalData = std::make_unique<unsigned char[]>(TotalSize);
			std::fill_n(InternalData.get(), TotalSize, (unsigned char) 0);
			// Perform the alignment calculation to know where we should consider the start of the data buffer
			void* RawBufferPtr = InternalData.get();
			std::align(Alignment, Size, RawBufferPtr, TotalSize);
			// store the offset required to align reads or writes to the 4k boundary
			AlignmentOffset = (unsigned char*) RawBufferPtr - InternalData.get();
		}

		Buffer::~Buffer()
		{
			InternalData.reset();
		}

		Buffer Buffer::CopyRange(std::size_t BeginIndex, std::size_t EndIndex)
		{
			Buffer RangedCopy = Buffer(EndIndex - BeginIndex, Alignment);
			std::copy(begin() + BeginIndex, begin() + EndIndex, RangedCopy.begin());
			return RangedCopy;
		}

		Buffer Buffer::CopyRange(const_iterator Start, const_iterator End)
		{
			Buffer RangedCopy = Buffer(End - Start, Alignment);
			std::copy(Start, End, RangedCopy.begin());
			return RangedCopy;
		}

		Buffer Buffer::Clone() const
		{
			Buffer MyClone = Buffer(Size, Alignment);
			std::copy(begin(), end(), MyClone.begin());
			return MyClone;
		}

		unsigned char* const Buffer::Data() const
		{
			return InternalData.get() + AlignmentOffset;
		}

		unsigned char* const Buffer::begin() const
		{
			return InternalData.get() + AlignmentOffset;
		}

		unsigned char* const Buffer::end() const
		{
			return InternalData.get() + AlignmentOffset + Size;
		}

		unsigned char& Buffer::operator[](size_t Index) const
		{
			return InternalData[Index + AlignmentOffset];
		}

		std::size_t Buffer::GetSize() const
		{
			return Size;
		}

		Buffer& Buffer::operator=(Buffer&& Source) noexcept
		{
			if (this != &Source)
			{
				InternalData = std::move(Source.InternalData);
				Size = std::move(Source.Size);
				Alignment = std::move(Source.Alignment);
				AlignmentOffset = std::move(Source.AlignmentOffset);
			}
			return *this;
		}

		std::vector<Modio::Detail::Buffer>::const_iterator DynamicBuffer::end() const
		{
			return InternalBuffers->end();
		}

		std::vector<Modio::Detail::Buffer>::iterator DynamicBuffer::end()
		{
			return InternalBuffers->end();
		}

		DynamicBuffer::DynamicBuffer(const DynamicBuffer& Other)
			: InternalBuffers(Other.InternalBuffers),
			  Alignment(Other.Alignment),
			  BufferLock(Other.BufferLock)
		{}

		DynamicBuffer::DynamicBuffer(std::size_t Alignment /*= 1*/)
			: InternalBuffers(std::make_shared<std::vector<Modio::Detail::Buffer>>()),
			  Alignment(Alignment),
			  BufferLock(std::make_shared<std::mutex>())
		{}

		Modio::Detail::DynamicBuffer DynamicBuffer::Clone() const
		{
			DynamicBuffer NewBuffer(Alignment);
			for (const Modio::Detail::Buffer& OriginalBuffer : *InternalBuffers)
			{
				// Make a copy of the buffer in other array
				NewBuffer.AppendBuffer(OriginalBuffer.Clone());
			}

			return std::move(NewBuffer);
		}

		void DynamicBuffer::CopyBufferConfiguration(const DynamicBuffer& Other)
		{
// @todo: When modio/ModioSDK.h is included after Windows.h when WINDOWS_LEAN_AND_MEAN is defined, we
// need to do this for packaged mode. There must be a nicer way to do it
#undef min

			Alignment = Other.Alignment;
			// Reconfigure overlapping buffers
			for (int Idx = 0; Idx < InternalBuffers->size() && Idx < Other.InternalBuffers->size(); ++Idx)
			{
				Modio::Detail::Buffer& MyBuffer = InternalBuffers->at(Idx);
				const Modio::Detail::Buffer& OtherBuffer = Other.InternalBuffers->at(Idx);
				if (MyBuffer.GetSize() != OtherBuffer.GetSize() ||
					MyBuffer.GetAlignment() != OtherBuffer.GetAlignment())
				{
					InternalBuffers->at(Idx) = Modio::Detail::Buffer(OtherBuffer.GetSize(), OtherBuffer.GetAlignment());
				}
			}

			// If we have more buffers than the other buffer, then reduce our size to others size
			while (InternalBuffers->size() > Other.InternalBuffers->size())
			{
				InternalBuffers->pop_back();
			}
			// The other has more buffers, create new buffers from that one
			if (InternalBuffers->size() < Other.InternalBuffers->size())
			{
				// Reserve memory upfront to minimize memory allocations
				InternalBuffers->reserve(Other.size());
				for (std::size_t Idx = std::min(InternalBuffers->size(), Other.InternalBuffers->size());
					 Idx < Other.InternalBuffers->size(); ++Idx)
				{
					const Modio::Detail::Buffer& OtherBuffer = Other.InternalBuffers->at(Idx);
					InternalBuffers->push_back(
						Modio::Detail::Buffer(OtherBuffer.GetSize(), OtherBuffer.GetAlignment()));
				}
			}
		}

		std::unique_lock<std::mutex> DynamicBuffer::Lock()
		{
			return std::move(std::unique_lock<std::mutex>(*BufferLock));
		}

		void DynamicBuffer::Clear()
		{
			InternalBuffers->clear();
		}

		std::size_t DynamicBuffer::size() const
		{
			std::size_t CumulativeSize = 0;

			for (Modio::Detail::Buffer& CurrentBuffer : *InternalBuffers)
			{
				if (CurrentBuffer.Data() == nullptr)
				{
					throw;
				}
				CumulativeSize += CurrentBuffer.GetSize();
			}
			return CumulativeSize;
		}

		std::size_t DynamicBuffer::max_size() const
		{
			// @todo: When modio/ModioSDK.h is included after Windows.h when WINDOWS_LEAN_AND_MEAN is defined,
			// we need to do this for packaged mode. There must be a nicer way to do it

#undef max

			return std::numeric_limits<std::size_t>::max();
		}

		std::size_t DynamicBuffer::capacity() const
		{
			return size();
		}

		Modio::Detail::DynamicBuffer::mutable_buffers_type DynamicBuffer::data()
		{
			return DynamicBufferSequence(InternalBuffers);
		}

		Modio::Detail::DynamicBuffer::const_buffers_type DynamicBuffer::data() const
		{
			return DynamicBufferSequence(InternalBuffers);
		}

		Modio::Detail::DynamicBuffer::mutable_buffers_type DynamicBuffer::data(std::size_t pos, std::size_t n)
		{
			return DynamicBufferSequence(InternalBuffers, pos, n);
		}

		Modio::Detail::DynamicBuffer::const_buffers_type DynamicBuffer::data(std::size_t pos, std::size_t n) const
		{
			return DynamicBufferSequence(InternalBuffers, pos, n);
		}

		void DynamicBuffer::grow(std::size_t n)
		{
			InternalBuffers->push_back(Modio::Detail::Buffer(n, Alignment));
		}

		void DynamicBuffer::shrink(std::size_t n)
		{
			if (InternalBuffers->size() == 0)
			{
				return;
			}
			// if n is bigger than the last buffer, erase the last buffer and continue to iterate backwards erasing
			// until n is less than the current tail buffer size or there are no buffers left
			if (n >= InternalBuffers->back().GetSize())
			{
				do
				{
					n -= InternalBuffers->back().GetSize();
					InternalBuffers->pop_back();
					if (InternalBuffers->size() == 0)
					{
						return;
					}

				} while (n >= InternalBuffers->back().GetSize());
			}

			if (n > 0)
			{
				// Create a new buffer that is a copy of the unerased bytes of the tail buffer and swap that in
				Modio::Detail::Buffer& OldTailBuffer = InternalBuffers->back();
				Modio::Detail::Buffer NewTailBuffer =
					OldTailBuffer.CopyRange(OldTailBuffer.begin(), OldTailBuffer.begin() + OldTailBuffer.GetSize() - n);
				std::swap(InternalBuffers->back(), NewTailBuffer);
			}
		}

		void DynamicBuffer::consume(std::size_t n)
		{
			if (InternalBuffers->size() == 0)
			{
				return;
			}
			if (n >= InternalBuffers->front().GetSize())
			{
				do
				{
					n -= InternalBuffers->front().GetSize();
					InternalBuffers->erase(InternalBuffers->begin());
					if (InternalBuffers->size() == 0)
					{
						return;
					}
				} while (n >= InternalBuffers->front().GetSize());
			}
			if (n > 0)
			{
				Modio::Detail::Buffer& OldHeadBuffer = InternalBuffers->front();
				Modio::Detail::Buffer NewHeadBuffer =
					OldHeadBuffer.CopyRange(OldHeadBuffer.begin() + n, OldHeadBuffer.end());
				std::swap(InternalBuffers->front(), NewHeadBuffer);
			}
		}

		Modio::Optional<Modio::Detail::Buffer> DynamicBuffer::TakeInternalBuffer()
		{
			if (InternalBuffers->size() == 0)
			{
				return {};
			}
			Modio::Detail::Buffer HeadBuffer = std::move(InternalBuffers->front());
			InternalBuffers->erase(InternalBuffers->begin());
			return HeadBuffer;
		}

		void DynamicBuffer::AppendBuffer(Modio::Detail::Buffer NewBuffer)
		{
			if (NewBuffer.GetAlignment() == Alignment)
			{
				InternalBuffers->push_back(std::move(NewBuffer));
			}
			else
			{
				Modio::Detail::Buffer AlignedCopy(NewBuffer.GetSize(), Alignment);
				std::copy(NewBuffer.begin(), NewBuffer.end(), AlignedCopy.begin());
				InternalBuffers->push_back(std::move(AlignedCopy));
			}
		}

		std::vector<Modio::Detail::Buffer>::const_iterator DynamicBuffer::begin() const
		{
			return InternalBuffers->begin();
		}

		std::vector<Modio::Detail::Buffer>::iterator DynamicBuffer::begin()
		{
			return InternalBuffers->begin();
		}

		bool DynamicBuffer::Equals(const Modio::Detail::DynamicBuffer& Other) const
		{
			// Does the buffers contain different amount of buffers
			if (InternalBuffers->size() != Other.InternalBuffers->size())
			{
				return false;
			}

			// Are all the internal buffers of the same size
			for (std::size_t Idx = 0; Idx < InternalBuffers->size(); ++Idx)
			{
				if ((*InternalBuffers)[Idx].GetSize() != (*Other.InternalBuffers)[Idx].GetSize())
				{
					return false;
				}
			}

			// Is the content of all the internal buffers the same
			for (std::size_t Idx = 0; Idx < InternalBuffers->size(); ++Idx)
			{
				if (std::memcmp((*InternalBuffers)[Idx].Data(), (*Other.InternalBuffers)[Idx].Data(),
								(*InternalBuffers)[Idx].GetSize()) != 0)
				{
					return false;
				}
			}

			return true;
		}

		DynamicBuffer::DynamicBufferSequence::DynamicBufferSequence(
			std::shared_ptr<std::vector<Modio::Detail::Buffer>> BuffersToView)
		{
			for (Modio::Detail::Buffer& CurrentBuffer : *BuffersToView)
			{
				BufferViews.push_back(Modio::MutableBufferView(CurrentBuffer.Data(), CurrentBuffer.GetSize()));
			}
		}

		DynamicBuffer::DynamicBufferSequence::DynamicBufferSequence(
			std::shared_ptr<std::vector<Modio::Detail::Buffer>> BuffersToView, std::size_t ByteOffset,
			std::size_t NumberOfBytes)
		{
			bool FirstBufferLocated = false;

			for (Modio::Detail::Buffer& CurrentBuffer : *BuffersToView)
			{
				if (!FirstBufferLocated && (ByteOffset < CurrentBuffer.GetSize()))
				{
					// Construct the first buffer view, containing the last n bytes of the current buffer from
					// the collection we're viewing
					FirstBufferLocated = true;
					std::size_t BytesInFirstBuffer = CurrentBuffer.GetSize() - ByteOffset;
					BufferViews.push_back(
						Modio::MutableBufferView(CurrentBuffer.Data() + ByteOffset, BytesInFirstBuffer));
					NumberOfBytes -= BytesInFirstBuffer;
				}
				else
				{
					if (NumberOfBytes < CurrentBuffer.GetSize())
					{
						// Construct the last buffer view, containing the first n bytes of the current buffer
						// from the collection we're viewing
						BufferViews.push_back(Modio::MutableBufferView(CurrentBuffer.Data(), NumberOfBytes));
						NumberOfBytes = 0;
					}
					else
					{
						// Construct another buffer view, consisting of the entire buffer from the collection
						// we're viewing
						BufferViews.push_back(Modio::MutableBufferView(CurrentBuffer.Data(), CurrentBuffer.GetSize()));
						NumberOfBytes -= CurrentBuffer.GetSize();
					}
				}
				// If we run out of bytes to represent in our view before we're done iterating through all the
				// buffers in the collection we're viewing, bail early
				if (NumberOfBytes == 0)
				{
					return;
				}
			}
		}

		std::vector<Modio::MutableBufferView>::const_iterator DynamicBuffer::DynamicBufferSequence::begin() const
		{
			return BufferViews.begin();
		}

		const std::vector<Modio::MutableBufferView>::const_iterator DynamicBuffer::DynamicBufferSequence::end() const
		{
			return BufferViews.end();
		}

	} // namespace Detail
} // namespace Modio
