"use strict";(self.webpackChunkmodio_docs=self.webpackChunkmodio_docs||[]).push([[1462],{20040:(e,n,t)=>{t.r(n),t.d(n,{assets:()=>d,contentTitle:()=>l,default:()=>c,frontMatter:()=>o,metadata:()=>r,toc:()=>a});var i=t(74848),s=t(28453);const o={id:"ue-installation-and-setup",title:"Installation & Setup",slug:"/unreal/installation-and-setup/",custom_edit_url:"https://github.com/modio/modio-ue4-internal/blob/develop/Plugins/Modio/Doc/installation-and-setup.mdx"},l=void 0,r={id:"ue-installation-and-setup",title:"Installation & Setup",description:"Adding plugin files to your project",source:"@site/public/en-us/installation-and-setup.mdx",sourceDirName:".",slug:"/unreal/installation-and-setup/",permalink:"/unreal/installation-and-setup/",draft:!1,unlisted:!1,editUrl:"https://github.com/modio/modio-ue4-internal/blob/develop/Plugins/Modio/Doc/installation-and-setup.mdx",tags:[],version:"current",frontMatter:{id:"ue-installation-and-setup",title:"Installation & Setup",slug:"/unreal/installation-and-setup/",custom_edit_url:"https://github.com/modio/modio-ue4-internal/blob/develop/Plugins/Modio/Doc/installation-and-setup.mdx"},sidebar:"sidebar",previous:{title:"Overview",permalink:"/unreal/"},next:{title:"Getting Started",permalink:"/unreal/getting-started/"}},d={},a=[{value:"Adding plugin files to your project",id:"adding-plugin-files-to-your-project",level:2},{value:"As a git submodule",id:"as-a-git-submodule",level:3},{value:"In a non-git project, or without submodules",id:"in-a-non-git-project-or-without-submodules",level:3},{value:"Enabling the plugin",id:"enabling-the-plugin",level:2},{value:"Plugin configuration",id:"plugin-configuration",level:2},{value:"Using a background thread",id:"using-a-background-thread",level:2},{value:"Using the <code>Live</code> environment",id:"using-the-live-environment",level:2},{value:"Next Steps",id:"next-steps",level:2}];function A(e){const n={a:"a",admonition:"admonition",code:"code",h2:"h2",h3:"h3",img:"img",li:"li",p:"p",strong:"strong",table:"table",tbody:"tbody",td:"td",th:"th",thead:"thead",tr:"tr",ul:"ul",...(0,s.R)(),...e.components};return(0,i.jsxs)(i.Fragment,{children:[(0,i.jsx)(n.h2,{id:"adding-plugin-files-to-your-project",children:"Adding plugin files to your project"}),"\n",(0,i.jsxs)(n.p,{children:["Download the Unreal Engine plugin from the ",(0,i.jsx)(n.a,{href:"https://github.com/modio/modio-ue",children:"mod.io GitHub repository"})," using one of the following methods:"]}),"\n",(0,i.jsx)(n.h3,{id:"as-a-git-submodule",children:"As a git submodule"}),"\n",(0,i.jsxs)(n.ul,{children:["\n",(0,i.jsxs)(n.li,{children:["\n",(0,i.jsxs)(n.p,{children:["In the directory with your ",(0,i.jsx)(n.code,{children:".uproject"})," file, add and download the mod.io plugin as a git submodule with the command:"]}),"\n",(0,i.jsx)(n.p,{children:(0,i.jsx)(n.code,{children:"git submodule add https://github.com/modio/modio-ue.git Plugins/Modio"})}),"\n"]}),"\n",(0,i.jsxs)(n.li,{children:["\n",(0,i.jsx)(n.p,{children:"Next, initialize our submodules and dependencies:"}),"\n",(0,i.jsx)(n.p,{children:(0,i.jsx)(n.code,{children:"git submodule update --init --recursive"})}),"\n"]}),"\n"]}),"\n",(0,i.jsx)(n.h3,{id:"in-a-non-git-project-or-without-submodules",children:"In a non-git project, or without submodules"}),"\n",(0,i.jsxs)(n.ul,{children:["\n",(0,i.jsxs)(n.li,{children:["Grab the latest release zip from the ",(0,i.jsx)(n.a,{href:"https://github.com/modio/modio-ue/releases",children:"releases section"})," on GitHub and extract the contents to your project\u2019s ",(0,i.jsx)(n.code,{children:"Plugins/Modio"})," directory"]}),"\n"]}),"\n",(0,i.jsxs)(n.p,{children:[(0,i.jsx)(n.img,{alt:"get_latest_release",src:t(73649).A+"",width:"385",height:"235"})," ",(0,i.jsx)(n.img,{alt:"get_latest_release2",src:t(77655).A+"",width:"366",height:"181"})]}),"\n",(0,i.jsx)(n.admonition,{type:"note",children:(0,i.jsx)(n.p,{children:"GitHub's automatically generated zips will not work! They do not contain our submodule dependencies. Ensure you download the zip file from the releases page as highlighted above."})}),"\n",(0,i.jsx)(n.h2,{id:"enabling-the-plugin",children:"Enabling the plugin"}),"\n",(0,i.jsx)(n.p,{children:"Start the editor and enable the plugin in the Unreal Engine plugin settings."}),"\n",(0,i.jsx)(n.p,{children:(0,i.jsx)(n.img,{alt:"plugin_window",src:t(80101).A+"",width:"1593",height:"633"})}),"\n",(0,i.jsx)(n.h2,{id:"plugin-configuration",children:"Plugin configuration"}),"\n",(0,i.jsx)(n.p,{children:"The plugin requires some initial configuration to behave correctly. You can access these settings in the Project Settings window:"}),"\n",(0,i.jsx)(n.p,{children:(0,i.jsx)(n.img,{alt:"plugin_settings",src:t(17834).A+"",width:"1577",height:"964"})}),"\n",(0,i.jsx)(n.p,{children:"The settings have the following parameters:"}),"\n",(0,i.jsxs)(n.table,{children:[(0,i.jsx)(n.thead,{children:(0,i.jsxs)(n.tr,{children:[(0,i.jsx)(n.th,{children:"Property"}),(0,i.jsx)(n.th,{children:"Description"})]})}),(0,i.jsxs)(n.tbody,{children:[(0,i.jsxs)(n.tr,{children:[(0,i.jsx)(n.td,{children:"Game Id"}),(0,i.jsx)(n.td,{children:"Your mod.io-provided Game Id for the target environment"})]}),(0,i.jsxs)(n.tr,{children:[(0,i.jsx)(n.td,{children:"API Key"}),(0,i.jsx)(n.td,{children:"Your mod.io-provided API key for the target environment"})]}),(0,i.jsxs)(n.tr,{children:[(0,i.jsx)(n.td,{children:"Environment"}),(0,i.jsxs)(n.td,{children:["Your target environment, ",(0,i.jsx)(n.code,{children:"Live"})," or ",(0,i.jsx)(n.code,{children:"Test"})]})]}),(0,i.jsxs)(n.tr,{children:[(0,i.jsx)(n.td,{children:"Log Level"}),(0,i.jsx)(n.td,{children:"The default logging level to use. Messages with a lower log level will be silently discarded."})]}),(0,i.jsxs)(n.tr,{children:[(0,i.jsx)(n.td,{children:"Portal"}),(0,i.jsx)(n.td,{children:"The default portal to use. This usually corresponds to the store your game will be distributed through."})]}),(0,i.jsxs)(n.tr,{children:[(0,i.jsx)(n.td,{children:"Use Background Thread"}),(0,i.jsxs)(n.td,{children:["Enables or disables the use of a ",(0,i.jsx)(n.a,{href:"#using-a-background-thread",children:"background thread"})," for the plugin's work."]})]})]})]}),"\n",(0,i.jsx)(n.admonition,{type:"note",children:(0,i.jsxs)(n.p,{children:["The Test environment is available for usage in limited circumstances only. ",(0,i.jsx)(n.strong,{children:(0,i.jsx)(n.a,{href:"/unreal/installation-and-setup/#using-the-live-environment",children:"All games should be set up on the Live environment"})}),". Set your game to ",(0,i.jsx)(n.strong,{children:"hidden"})," to restrict access during the testing phase. If you require access to Test, please contact us."]})}),"\n",(0,i.jsx)(n.h2,{id:"using-a-background-thread",children:"Using a background thread"}),"\n",(0,i.jsxs)(n.p,{children:[(0,i.jsx)(n.a,{href:"https://docs.mod.io/unrealref/#RunPendingHandlers",children:(0,i.jsx)(n.code,{children:"RunPendingHandlers"})})," is responsible for running any pending plugin work, including invoking callbacks passed to asynchronous operations.  With ",(0,i.jsx)(n.code,{children:"Use Background Thread"}),"  set to ",(0,i.jsx)(n.code,{children:"true"}),", the mod.io plugin will automatically spin up a background thread and continually call ",(0,i.jsx)(n.code,{children:"RunPendingHandlers"})," for you. This decouples the frequency of the plugin\u2019s work from your game's main thread, and improves performance of the plugin overall."]}),"\n",(0,i.jsxs)(n.p,{children:["Calling methods from the ",(0,i.jsx)(n.a,{href:"https://docs.mod.io/unrealref/#_modiosubsystem",children:(0,i.jsx)(n.code,{children:"ModioSubsystem"})})," will marshall all callbacks back to the game thread.  All other plugin call guarantees remain the same."]}),"\n",(0,i.jsxs)(n.h2,{id:"using-the-live-environment",children:["Using the ",(0,i.jsx)(n.code,{children:"Live"})," environment"]}),"\n",(0,i.jsxs)(n.p,{children:["All games should be set up on the ",(0,i.jsx)(n.code,{children:"Live"})," environment for full access to the mod.io REST API's capabilities."]}),"\n",(0,i.jsxs)(n.p,{children:["If your game or it's modding capabilities are not yet publicly available, set your game to ",(0,i.jsx)(n.strong,{children:"hidden"})," on the ",(0,i.jsx)(n.code,{children:"Live"})," environment.  This allows you to restrict access to specific accounts and perform QA against the production environment without exposing your title to the public."]}),"\n",(0,i.jsxs)(n.p,{children:["Once you are ready, change the state from ",(0,i.jsx)(n.strong,{children:"hidden"})," to ",(0,i.jsx)(n.strong,{children:"public"})," for a full release of your game\u2019s modding capabilities."]}),"\n",(0,i.jsx)(n.h2,{id:"next-steps",children:"Next Steps"}),"\n",(0,i.jsxs)(n.ul,{children:["\n",(0,i.jsxs)(n.li,{children:["Our ",(0,i.jsx)(n.a,{href:"/unreal/getting-started/",children:"Getting Started Guide"})," page contains a series of quick-start guides with code samples demonstrating the plugin's core functionality."]}),"\n",(0,i.jsxs)(n.li,{children:["A sample project demonstrating basic mod.io functionality is available ",(0,i.jsx)(n.a,{href:"https://go.mod.io/ue5-sample",children:"here"}),"."]}),"\n"]})]})}function c(e={}){const{wrapper:n}={...(0,s.R)(),...e.components};return n?(0,i.jsx)(n,{...e,children:(0,i.jsx)(A,{...e})}):A(e)}},73649:(e,n,t)=>{t.d(n,{A:()=>i});const i="data:image/png;base64,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"},77655:(e,n,t)=>{t.d(n,{A:()=>i});const i="data:image/png;base64,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"},17834:(e,n,t)=>{t.d(n,{A:()=>i});const i=t.p+"assets/images/plugin_settings-031f6d03534360a3799d7adbf8180173.png"},80101:(e,n,t)=>{t.d(n,{A:()=>i});const i=t.p+"assets/images/plugin_window-74a3e99e913b1314a247702723f4af28.png"},28453:(e,n,t)=>{t.d(n,{R:()=>l,x:()=>r});var i=t(96540);const s={},o=i.createContext(s);function l(e){const n=i.useContext(o);return i.useMemo((function(){return"function"==typeof e?e(n):{...n,...e}}),[n,e])}function r(e){let n;return n=e.disableParentContext?"function"==typeof e.components?e.components(s):e.components||s:l(e.components),i.createElement(o.Provider,{value:n},e.children)}}}]);