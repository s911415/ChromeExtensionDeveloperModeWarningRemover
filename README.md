# ChromeExtensionDeveloperModeWarningRemover
A tool to remove developer mode extensions warning in Google Chrome x64.

This tool have tested with 75.0.3770.18.
For earlier version, please switch to v1.1 (tested with v64.0.3282.186 and v65.0.3325.106).

## How to use
Compile source codes or download binary from release page.

Make sure chrome is closed before run this tool.

Run following command in Command Prompt.

```console
ChromeExtensionDeveloperModeWarningRemover.exe "%ProgramFiles(x86)%\Google\Chrome\Application\{version}\chrome.dll"
```

 
## References

- [吾爱破解 - 谷歌浏览器去除开发者模式运行扩展程序提示](https://www.52pojie.cn/thread-695123-1-3.html)
- [Stack Overflow - How to get rid of “disable developer mode extensions” pop-up](https://stackoverflow.com/questions/30287907/how-to-get-rid-of-disable-developer-mode-extensions-pop-up/30361260#30361260)
- [Chromium - chrome/browser/ui/extensions/extension_message_bubble_factory.cc](https://chromium.googlesource.com/chromium/src/+/refs/tags/75.0.3770.18/chrome/browser/ui/extensions/extension_message_bubble_factory.cc#66)
