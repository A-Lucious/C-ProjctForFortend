# 项目记录（更新）

**概览**

这是一个基于 **C++ + Crow** 的后台服务项目，采用 **CMake** 构建。后端负责提供 JSON API 与静态文件（前端页面），前端使用原生 JavaScript（fetch + async/await）和 **ECharts** 进行数据可视化。

---

## 项目结构 🔍

- 根目录
  - `CMakeLists.txt`：构建脚本
  - `helloworld.cpp`：示例/入口文件
- `build/`：CMake 生成的构建输出
  - `HelloWorld`：可执行文件（运行入口）
  - `compile_commands.json`：供 IDE/静态分析使用
  - `Testing/`：CTest 测试运行结果
- `frontend/`
  - `index.html`、`script.js`、`style.css`：前端静态资源，后端以静态文件方式提供给浏览器
- `Example/`
  - `courses.json`、`students.json`、`teachers.json`：示例数据
- `src/`
  - `lib/`：项目源码（`course.cpp`、`functions.cpp` 等）及头文件（`course.h`、`functions.h`、`college.h` 等）

---

## 已实现的功能 ✅

- 后端
  - 使用 Crow 定义路由（`CROW_ROUTE`）并暴露 API（GET/POST）
  - 静态文件分发：向浏览器提供 `index.html`、`script.js`、`style.css`
  - JSON 交互：接收 `crow::json::load(req.body)` 并返回 `crow::json::wvalue`
  - 处理 CORS（添加 `Access-Control-Allow-*` 响应头并处理 `OPTIONS` 预检请求）
- 前端
  - 使用 `fetch`、`async/await` 实现异步请求（AJAX）
  - 使用 ECharts 将后端返回的数值数组展现为动态图表
- 构建/测试
  - 使用 CMake 生成构建系统，生成 `compile_commands.json` 以便 IDE 支持
  - 可通过 CTest（在 `build/Testing` 中）查看测试结果
****
---

## 构建与运行（快速指南） ▶️

1. 生成构建文件：
   ```bash
   cmake -S . -B build
   ```
2. 编译：
   ```bash
   cmake --build build -- -j
   ```
3. 运行（在 `build/` 中运行，以保证相对路径正确）：
   ```bash
   ./build/HelloWorld
   ```

> 提示：`compile_commands.json` 可用于代码导航和静态分析；确保在 IDE 中启用该文件以获得更好体验。

---

## 后续改进建议 ✍️

- 在 `README.md` 中补充 API 文档（列出路由、请求示例、返回格式）
- 将静态文件路径通过配置注入，避免依赖当前工作目录
- 增加单元测试覆盖 `src/lib` 的关键函数，并将测试纳入 CI

---

*记录更新时间：仅为反映当前仓库结构与已实现的核心功能。如需扩展为更完备的开发文档或 API 参考，请告诉我具体要包含的内容，我可以帮你生成。