"use strict";(self.webpackChunkmodio_docs=self.webpackChunkmodio_docs||[]).push([[3124],{19169:(e,t,n)=>{n.r(t),n.d(t,{assets:()=>u,contentTitle:()=>l,default:()=>b,frontMatter:()=>r,metadata:()=>d,toc:()=>c});var o=n(74848),i=n(28453),a=n(27064),s=n(89236);const r={id:"ue-submit-mods",title:"In-Game Mod Submission",slug:"/unreal/getting-started/submit-mods",custom_edit_url:"https://github.com/modio/modio-ue4-internal/blob/develop/Plugins/Modio/Doc/doc_root/en-us/getting-started/submit-mods.mdx"},l=void 0,d={id:"getting-started/ue-submit-mods",title:"In-Game Mod Submission",description:"Submitting a mod from inside your game and making it visible to other players involves two steps:",source:"@site/public/en-us/getting-started/submit-mods.mdx",sourceDirName:"getting-started",slug:"/unreal/getting-started/submit-mods",permalink:"/unreal/getting-started/submit-mods",draft:!1,unlisted:!1,editUrl:"https://github.com/modio/modio-ue4-internal/blob/develop/Plugins/Modio/Doc/doc_root/en-us/getting-started/submit-mods.mdx",tags:[],version:"current",frontMatter:{id:"ue-submit-mods",title:"In-Game Mod Submission",slug:"/unreal/getting-started/submit-mods",custom_edit_url:"https://github.com/modio/modio-ue4-internal/blob/develop/Plugins/Modio/Doc/doc_root/en-us/getting-started/submit-mods.mdx"},sidebar:"sidebar",previous:{title:"Mod Subscriptions & Management",permalink:"/unreal/getting-started/mod-subscriptions"},next:{title:"Edit an Existing Mod",permalink:"/unreal/getting-started/edit-mods"}},u={},c=[{value:"Submitting a new mod",id:"submitting-a-new-mod",level:2},{value:"Submitting a file for a mod",id:"submitting-a-file-for-a-mod",level:2}];function m(e){const t={a:"a",admonition:"admonition",code:"code",h2:"h2",img:"img",li:"li",p:"p",pre:"pre",strong:"strong",ul:"ul",...(0,i.R)(),...e.components};return(0,o.jsxs)(o.Fragment,{children:[(0,o.jsx)(t.p,{children:"Submitting a mod from inside your game and making it visible to other players involves two steps:"}),"\n",(0,o.jsxs)(t.ul,{children:["\n",(0,o.jsx)(t.li,{children:"Submission of the mod"}),"\n",(0,o.jsx)(t.li,{children:'Submission of the mod\u2019s data (aka "the mod file")'}),"\n"]}),"\n",(0,o.jsxs)(t.p,{children:["These steps are outlined below. Mods can also be edited after submission, as ",(0,o.jsx)(t.a,{href:"edit-mods",children:"detailed here"})]}),"\n",(0,o.jsx)(t.h2,{id:"submitting-a-new-mod",children:"Submitting a new mod"}),"\n",(0,o.jsxs)(t.p,{children:["To submit a mod, you must first create a mod handle using ",(0,o.jsx)(t.a,{href:"/unreal/refdocs/#getmodcreationhandle",children:(0,o.jsx)(t.code,{children:"GetModCreationHandle"})}),", and use that handle when calling ",(0,o.jsx)(t.a,{href:"/unreal/refdocs/#submitnewmodasync",children:(0,o.jsx)(t.code,{children:"SubmitNewModAsync"})}),". Note that the newly created mod will remain hidden until a mod file is added in the next step."]}),"\n",(0,o.jsxs)(a.A,{"group-id":"languages",children:[(0,o.jsx)(s.A,{value:"blueprint",label:"Blueprint",children:(0,o.jsx)(t.p,{children:(0,o.jsx)(t.img,{alt:"submit_new_mod",src:n(92695).A+"",width:"870",height:"519"})})}),(0,o.jsx)(s.A,{value:"c++",label:"C++",default:!0,children:(0,o.jsx)(t.pre,{children:(0,o.jsx)(t.code,{className:"language-cpp",children:'void UModioManagerSubsystem::SubmitNewMod()\n{\n   if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())\n   {\n   \tFModioModCreationHandle Handle = Subsystem->GetModCreationHandle();\n\n   \tFModioCreateModParams Params;\n   \tParams.Name = TEXT("My Amazing Mod");\n   \tParams.Description = TEXT("This mod does amazing things");\n   \tParams.PathToLogoFile = TEXT("C:\\\\path\\\\to\\\\image.png");\n\n   \tSubsystem->SubmitNewModAsync(Handle, Params, FOnSubmitNewModDelegateFast::CreateUObject(this, &UModioManagerSubsystem::OnSubmitNewModComplete));\n   }\n}\n\nvoid UModioManagerSubsystem::OnSubmitNewModComplete(FModioErrorCode ErrorCode, TOptional<FModioModID> ModId)\n{\n   if (!ErrorCode)\n   {\n   \t// Mod was successfully submitted \n   \t// We can now call SubmitNewModFileForMod with this ModId\n   }\n}\n'})})})]}),"\n",(0,o.jsx)(t.h2,{id:"submitting-a-file-for-a-mod",children:"Submitting a file for a mod"}),"\n",(0,o.jsxs)(t.p,{children:["Once you have successfully submitted a mod, you can submit a mod file for that mod using ",(0,o.jsx)(t.a,{href:"/unreal/refdocs/#submitnewmodfileformod",children:(0,o.jsx)(t.code,{children:"SubmitNewModFileForMod"})}),". When you submit a mod file, you pass a ",(0,o.jsx)(t.a,{href:"/unreal/refdocs/#modiocreatemodfileparams",children:(0,o.jsx)(t.code,{children:"ModioCreateModFileParams"})})," containing the directory containing all the files that you want to submit. The plugin will compress this folder into a .zip file and upload it as the active version of the mod."]}),"\n",(0,o.jsxs)(t.p,{children:["In the future, if the mod is updated and requires a new mod file, ",(0,o.jsx)(t.code,{children:"SubmitNewModFileForMod"})," can be called again. The most recent mod file uploaded by ",(0,o.jsx)(t.code,{children:"SubmitNewModFileForMod"})," will be set as the active version."]}),"\n",(0,o.jsx)(t.admonition,{type:"note",children:(0,o.jsxs)(t.p,{children:["There is no callback for ",(0,o.jsx)(t.code,{children:"SubmitNewModFileForMod"}),"; you\u2019ll be notified of the completed upload by your ",(0,o.jsx)(t.a,{href:"mod-subscriptions#installation-management",children:(0,o.jsx)(t.strong,{children:"mod management callback"})}),"."]})}),"\n",(0,o.jsxs)(a.A,{"group-id":"languages",children:[(0,o.jsxs)(s.A,{value:"blueprint",label:"Blueprint",children:[(0,o.jsx)(t.p,{children:"After the callback for submitting a mod has completed, you can get the Mod Id to use for file submission."}),(0,o.jsx)(t.p,{children:(0,o.jsx)(t.img,{alt:"submit_new_mod_file",src:n(12214).A+"",width:"862",height:"483"})})]}),(0,o.jsx)(s.A,{value:"c++",label:"C++",default:!0,children:(0,o.jsx)(t.pre,{children:(0,o.jsx)(t.code,{className:"language-cpp",children:'void UModioManagerSubsystem::SubmitNewModFile(FModioModID ModId)\n{\n   if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())\n   {\n   \tFModioCreateModFileParams Params;\n   \tParams.PathToModRootDirectory = TEXT("C:\\\\path\\\\to\\\\mod-folder");\n   \tSubsystem->SubmitNewModFileForMod(ModId, Params);\n   }\n}\n'})})})]})]})}function b(e={}){const{wrapper:t}={...(0,i.R)(),...e.components};return t?(0,o.jsx)(t,{...e,children:(0,o.jsx)(m,{...e})}):m(e)}},19365:(e,t,n)=>{n.d(t,{A:()=>s});n(96540);var o=n(18215);const i={tabItem:"tabItem_Ymn6"};var a=n(74848);function s(e){let{children:t,hidden:n,className:s}=e;return(0,a.jsx)("div",{role:"tabpanel",className:(0,o.A)(i.tabItem,s),hidden:n,children:t})}},11470:(e,t,n)=>{n.d(t,{A:()=>y});var o=n(96540),i=n(18215),a=n(23104),s=n(56347),r=n(205),l=n(57485),d=n(31682),u=n(70679);function c(e){return o.Children.toArray(e).filter((e=>"\n"!==e)).map((e=>{if(!e||(0,o.isValidElement)(e)&&function(e){const{props:t}=e;return!!t&&"object"==typeof t&&"value"in t}(e))return e;throw new Error(`Docusaurus error: Bad <Tabs> child <${"string"==typeof e.type?e.type:e.type.name}>: all children of the <Tabs> component should be <TabItem>, and every <TabItem> should have a unique "value" prop.`)}))?.filter(Boolean)??[]}function m(e){const{values:t,children:n}=e;return(0,o.useMemo)((()=>{const e=t??function(e){return c(e).map((e=>{let{props:{value:t,label:n,attributes:o,default:i}}=e;return{value:t,label:n,attributes:o,default:i}}))}(n);return function(e){const t=(0,d.X)(e,((e,t)=>e.value===t.value));if(t.length>0)throw new Error(`Docusaurus error: Duplicate values "${t.map((e=>e.value)).join(", ")}" found in <Tabs>. Every value needs to be unique.`)}(e),e}),[t,n])}function b(e){let{value:t,tabValues:n}=e;return n.some((e=>e.value===t))}function h(e){let{queryString:t=!1,groupId:n}=e;const i=(0,s.W6)(),a=function(e){let{queryString:t=!1,groupId:n}=e;if("string"==typeof t)return t;if(!1===t)return null;if(!0===t&&!n)throw new Error('Docusaurus error: The <Tabs> component groupId prop is required if queryString=true, because this value is used as the search param name. You can also provide an explicit value such as queryString="my-search-param".');return n??null}({queryString:t,groupId:n});return[(0,l.aZ)(a),(0,o.useCallback)((e=>{if(!a)return;const t=new URLSearchParams(i.location.search);t.set(a,e),i.replace({...i.location,search:t.toString()})}),[a,i])]}function f(e){const{defaultValue:t,queryString:n=!1,groupId:i}=e,a=m(e),[s,l]=(0,o.useState)((()=>function(e){let{defaultValue:t,tabValues:n}=e;if(0===n.length)throw new Error("Docusaurus error: the <Tabs> component requires at least one <TabItem> children component");if(t){if(!b({value:t,tabValues:n}))throw new Error(`Docusaurus error: The <Tabs> has a defaultValue "${t}" but none of its children has the corresponding value. Available values are: ${n.map((e=>e.value)).join(", ")}. If you intend to show no default tab, use defaultValue={null} instead.`);return t}const o=n.find((e=>e.default))??n[0];if(!o)throw new Error("Unexpected error: 0 tabValues");return o.value}({defaultValue:t,tabValues:a}))),[d,c]=h({queryString:n,groupId:i}),[f,g]=function(e){let{groupId:t}=e;const n=function(e){return e?`docusaurus.tab.${e}`:null}(t),[i,a]=(0,u.Dv)(n);return[i,(0,o.useCallback)((e=>{n&&a.set(e)}),[n,a])]}({groupId:i}),p=(()=>{const e=d??f;return b({value:e,tabValues:a})?e:null})();(0,r.A)((()=>{p&&l(p)}),[p]);return{selectedValue:s,selectValue:(0,o.useCallback)((e=>{if(!b({value:e,tabValues:a}))throw new Error(`Can't select invalid tab value=${e}`);l(e),c(e),g(e)}),[c,g,a]),tabValues:a}}var g=n(92303);const p={tabList:"tabList__CuJ",tabItem:"tabItem_LNqP"};var v=n(74848);function x(e){let{className:t,block:n,selectedValue:o,selectValue:s,tabValues:r}=e;const l=[],{blockElementScrollPositionUntilNextRender:d}=(0,a.a_)(),u=e=>{const t=e.currentTarget,n=l.indexOf(t),i=r[n].value;i!==o&&(d(t),s(i))},c=e=>{let t=null;switch(e.key){case"Enter":u(e);break;case"ArrowRight":{const n=l.indexOf(e.currentTarget)+1;t=l[n]??l[0];break}case"ArrowLeft":{const n=l.indexOf(e.currentTarget)-1;t=l[n]??l[l.length-1];break}}t?.focus()};return(0,v.jsx)("ul",{role:"tablist","aria-orientation":"horizontal",className:(0,i.A)("tabs",{"tabs--block":n},t),children:r.map((e=>{let{value:t,label:n,attributes:a}=e;return(0,v.jsx)("li",{role:"tab",tabIndex:o===t?0:-1,"aria-selected":o===t,ref:e=>l.push(e),onKeyDown:c,onClick:u,...a,className:(0,i.A)("tabs__item",p.tabItem,a?.className,{"tabs__item--active":o===t}),children:n??t},t)}))})}function w(e){let{lazy:t,children:n,selectedValue:i}=e;const a=(Array.isArray(n)?n:[n]).filter(Boolean);if(t){const e=a.find((e=>e.props.value===i));return e?(0,o.cloneElement)(e,{className:"margin-top--md"}):null}return(0,v.jsx)("div",{className:"margin-top--md",children:a.map(((e,t)=>(0,o.cloneElement)(e,{key:t,hidden:e.props.value!==i})))})}function M(e){const t=f(e);return(0,v.jsxs)("div",{className:(0,i.A)("tabs-container",p.tabList),children:[(0,v.jsx)(x,{...t,...e}),(0,v.jsx)(w,{...t,...e})]})}function y(e){const t=(0,g.A)();return(0,v.jsx)(M,{...e,children:c(e.children)},String(t))}},89236:(e,t,n)=>{n.d(t,{A:()=>a});var o=n(19365),i=(n(96540),n(74848));function a(e){return(0,i.jsx)(i.Fragment,{children:(0,i.jsx)(o.A,{className:"tw-rounded-md",...e})})}},27064:(e,t,n)=>{n.d(t,{A:()=>a});var o=n(11470),i=(n(96540),n(74848));function a(e){return(0,i.jsx)("div",{className:"tw-border tw-border-solid tw-border-skyblue tw-rounded-md tw-bg-darkslategray tw-p-4 tw-mb-4",children:(0,i.jsx)(o.A,{...e})})}},92695:(e,t,n)=>{n.d(t,{A:()=>o});const o=n.p+"assets/images/submit_new_mod-d860f68584f2f0b67dd74c6e95467fcb.png"},12214:(e,t,n)=>{n.d(t,{A:()=>o});const o=n.p+"assets/images/submit_new_mod_file-e1cde8dca73f1d2d5985433981e8c5a3.png"},28453:(e,t,n)=>{n.d(t,{R:()=>s,x:()=>r});var o=n(96540);const i={},a=o.createContext(i);function s(e){const t=o.useContext(a);return o.useMemo((function(){return"function"==typeof e?e(t):{...t,...e}}),[t,e])}function r(e){let t;return t=e.disableParentContext?"function"==typeof e.components?e.components(i):e.components||i:s(e.components),o.createElement(a.Provider,{value:t},e.children)}}}]);