# IMETool

Windows 下的命令行输入法模式切换工具，用于在中/英文输入模式之间快速切换。专为微软拼音输入法设计，适配 VSCode Vim 插件等需要自动切换输入法的场景。

> 受 [im-select](https://github.com/daipeihust/im-select) 项目启发。

## 使用方法

```powershell
# 查询当前输入模式（输出 "en" 或 "zh"）
imetool

# 切换到英文模式
imetool en

# 切换到中文模式
imetool zh
```

## 构建

需要 **CMake** 和 **Visual Studio 2022**。

```powershell
cmake --preset vs -S . -B build 
cd build
cmake --build .
cmake --build . --config Release -t install
```

当前项目目录下的`imetool`目录即为构建产物

## VSCode Vim 插件配置

在 VSCode `settings.json` 中添加：

```json
"vim.autoSwitchInputMethod.enable": true,
"vim.autoSwitchInputMethod.defaultIM": "en",
"vim.autoSwitchInputMethod.obtainIMCmd": "/path/to/imetool.exe",
"vim.autoSwitchInputMethod.switchIMCmd": "/path/to/imetool.exe {im}"
```

退出插入模式时自动切换到英文，进入插入模式时恢复之前的输入状态。

## Obsidian

使用 [vim-im-select-obsidian](https://github.com/ALONELUR/vim-im-select-obsidian) 配置。
