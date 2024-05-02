using UnrealBuildTool;
using System;
using System.IO;
using System.Collections.Generic;

#if UE_5_0_OR_LATER
using EpicGames.Core;
#else
using Tools.DotNETCommon;
#endif

public class mbedtls : ModuleRules
{
    public mbedtls(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        bUseUnity = false;
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine" });

        PublicIncludePaths.AddRange(new string[] { Path.Combine(ModuleDirectory, "Public"), Path.Combine(ModuleDirectory, "../ThirdParty/mbedtls/include") });
        PrivateIncludePaths.AddRange(new string[] { Path.Combine(ModuleDirectory, "Private"), Path.Combine(ModuleDirectory, "../ThirdParty/mbedtls/library") });

        {
	        List<string> CFiles = new List<string>
	        {
		        "aes.c",
		        "aesni.c",
		        "aria.c",
		        "asn1parse.c",
		        "asn1write.c",
		        "base64.c",
		        "bignum.c",
		        "camellia.c",
		        "ccm.c",
		        "chacha20.c",
		        "chachapoly.c",
		        "cipher.c",
		        "cipher_wrap.c",
		        "cmac.c",
		        "constant_time.c",
		        "ctr_drbg.c",
		        "des.c",
		        "dhm.c",
		        "ecdh.c",
		        "ecdsa.c",
		        "ecjpake.c",
		        "ecp.c",
		        "ecp_curves.c",
		        "entropy.c",
		        "entropy_poll.c",
		        //"error.c",
		        "gcm.c",
		        "hkdf.c",
		        "hmac_drbg.c",
		        "md.c",
		        "md5.c",
		        "memory_buffer_alloc.c",
		        "mps_reader.c",
		        "mps_trace.c",
		        "nist_kw.c",
		        "oid.c",
		        "padlock.c",
		        "pem.c",
		        "pk.c",
		        "pk_wrap.c",
		        "pkcs12.c",
		        "pkcs5.c",
		        "pkparse.c",
		        "pkwrite.c",
		        "platform.c",
		        "platform_util.c",
		        "poly1305.c",
		        "psa_crypto.c",
		        "psa_crypto_aead.c",
		        "psa_crypto_cipher.c",
		        "psa_crypto_client.c",
		        "psa_crypto_driver_wrappers.c",
		        "psa_crypto_ecp.c",
		        "psa_crypto_hash.c",
		        "psa_crypto_mac.c",
		        "psa_crypto_rsa.c",
		        "psa_crypto_se.c",
		        "psa_crypto_slot_management.c",
		        "psa_crypto_storage.c",
		        "psa_its_file.c",
		        "ripemd160.c",
		        "rsa.c",
		        "rsa_alt_helpers.c",
		        "sha1.c",
		        "sha256.c",
		        "sha512.c",
		        "threading.c",
		        "timing.c",
		        "version.c",
		        //"version_features.c",
		        "x509.c",
		        "x509_create.c",
		        "x509_crl.c",
		        "x509_crt.c",
		        "x509_csr.c",
		        "x509write_crt.c",
		        "x509write_csr.c",
		        "debug.c",
		        "net_sockets.c",
		        "ssl_cache.c",
		        "ssl_ciphersuites.c",
		        "ssl_cli.c",
		        "ssl_cookie.c",
		        "ssl_msg.c",
		        "ssl_srv.c",
		        "ssl_ticket.c",
		        "ssl_tls.c",
		        "ssl_tls13_keys.c",
		        "ssl_tls13_server.c",
		        "ssl_tls13_client.c",
		        "ssl_tls13_generic.c"
	        };

	        // Delete the old Generated Source path if its present, as it may conflict with the new way we add GeneratedSource for this module
	        string OldGeneratedSourcePath = Path.Combine(ModuleDirectory, "Private/GeneratedSource");
	        if (Directory.Exists(OldGeneratedSourcePath))
	        {
		        Directory.Delete(OldGeneratedSourcePath, true);
	        }

	        // Set up the new GeneratedSourcePath. This folder sits outside the module directory so we can use
	        // ConditionalAddModuleDirectory. Relying on UBT to pull in files that we have copied via this script
	        // is unreliable and causes intermittent build issues.
	        string GeneratedSourcePath = Path.Combine(ModuleDirectory, "../ThirdParty", "GeneratedMbedtlsSource");

	        if (Directory.Exists(GeneratedSourcePath))
	        {
		        Directory.Delete(GeneratedSourcePath, true);
	        }

	        Directory.CreateDirectory(GeneratedSourcePath);

	        foreach (string CFile in CFiles)
	        {
		        //Add the original file in our upstream repository as a dependency, so if a user edits it we will copy it over
		        string SourcePath = Path.Combine(ModuleDirectory, "../ThirdParty/mbedtls/library", CFile);
		        //ExternalDependencies.Add(SourcePath);
		        string DestinationPath = Path.Combine(GeneratedSourcePath, CFile);
		        File.Copy(SourcePath, DestinationPath, true);
	        }

	        ConditionalAddModuleDirectory(new DirectoryReference(GeneratedSourcePath));
        }
    }
}