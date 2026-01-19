# 课程修改侧边栏实施计划

## 项目概述
为 C++ 通讯工程项目的前端添加"课程修改"功能，通过点击课程卡片弹出侧边栏进行编辑。

## 背景分析
- **后端**: C++ 层已实现 `/api/Set/Course` 接口
- **JS 逻辑**: `script.js` 中已有 `CourseSet()` 方法和 `setitem`, `setType`, `setQuery` 变量
- **现状**: 仅需修改 HTML 和 CSS，激活已有的修改功能

## 当前代码状态
### 1. HTML 结构
- **课程列表位置**: `index.html` 第 101 行
- **现有侧边栏**: 学生详情侧边栏使用 `.swap` 类，通过 `stuisOpen` 控制

### 2. CSS 样式
- **问题**: `style.css` 中存在重复的 `.swap` 定义（需清理）
- **侧边栏动画**: 使用 `position: fixed` + `right: -50%` 实现滑入效果

### 3. JS 变量
- **已定义但未使用**: `setType`, `setQuery`, `setitem` (第 37-40 行)
- **BUG**: `CourseSet()` 和 `StudentSet()` 中存在 `JSOM.stringify` 拼写错误（应为 `JSON.stringify`）

---

## 实施计划

### 任务 1: 修改 `frontend/index.html`

#### 1.1 添加课程列表点击事件
**位置**: 第 101 行的课程列表 `<div>`

**原代码**:
```html
<div v-for="item in courselist" :key="item.id" class="info-card">
```

**修改为**:
```html
<div 
    v-for="item in courselist" 
    :key="item.id" 
    class="info-card" 
    @click="setitem = item; courseEditOpen = true"
    style="cursor: pointer;"
>
```

#### 1.2 添加课程编辑侧边栏
**位置**: 在现有学生侧边栏（`<div class="swap" :class="{ 'swap-open': stuisOpen }">`）**之后**插入

**新增代码**:
```html
<!-- 课程修改侧边栏 -->
<div class="swap course-swap" :class="{ 'swap-open': courseEditOpen }">
    <div class="swap-header">
        <h3>修改课程: {{ setitem?.classname || '未选择' }}</h3>
        <button @click="courseEditOpen = false; setitem = null">×</button>
    </div>
    
    <div class="edit-container" v-if="setitem">
        <div class="form-group">
            <label>课程 ID</label>
            <input type="text" v-model="setitem.id" disabled>
        </div>
        
        <div class="form-group">
            <label>课程名称</label>
            <input type="text" v-model="setitem.classname">
        </div>
        
        <div class="form-group">
            <label>学分</label>
            <input type="number" v-model.number="setitem.credit" min="0" max="10">
        </div>
        
        <div class="form-group">
            <label>教师 ID</label>
            <input type="text" v-model="setitem.teacherid">
        </div>
        
        <div class="form-group">
            <label>GPA 权重</label>
            <input type="number" v-model.number="setitem.GPA" min="0" max="5" step="0.1">
        </div>
        
        <button class="btn-primary" @click="CourseSet(); courseEditOpen = false;">
            保存修改
        </button>
    </div>
</div>
```

---

### 任务 2: 修改 `frontend/style.css`

#### 2.1 清理重复的 `.swap` 定义
**操作**: 找到所有 `.swap` 定义，合并为一个

**最终样式**:
```css
.swap {
    position: fixed;
    right: -50%;
    top: 0;
    width: 45%;
    height: 100%;
    background: white;
    box-shadow: -5px 0 20px rgba(0,0,0,0.15);
    transition: right 0.35s cubic-bezier(0.4, 0, 0.2, 1);
    padding: 25px;
    z-index: 1000;
    overflow-y: auto;
}

.swap-open {
    right: 0;
}

.swap-header {
    display: flex;
    justify-content: space-between;
    align-items: center;
    margin-bottom: 20px;
    padding-bottom: 15px;
    border-bottom: 2px solid #e0e0e0;
}

.swap-header h3 {
    margin: 0;
    color: #333;
}

.swap-header button {
    background: none;
    border: none;
    font-size: 32px;
    cursor: pointer;
    color: #999;
    line-height: 1;
    padding: 0;
    width: 32px;
    height: 32px;
}

.swap-header button:hover {
    color: #333;
}
```

#### 2.2 添加表单样式
```css
/* 编辑表单容器 */
.edit-container {
    padding-top: 10px;
}

.form-group {
    margin-bottom: 20px;
}

.form-group label {
    display: block;
    margin-bottom: 8px;
    font-weight: 600;
    color: #555;
    font-size: 14px;
}

.form-group input {
    width: 100%;
    padding: 10px 12px;
    border: 1px solid #ddd;
    border-radius: 6px;
    font-size: 14px;
    transition: border-color 0.2s;
    box-sizing: border-box;
}

.form-group input:focus {
    outline: none;
    border-color: #4CAF50;
}

.form-group input:disabled {
    background-color: #f5f5f5;
    cursor: not-allowed;
}

/* 保存按钮 */
.btn-primary {
    background-color: #4CAF50;
    color: white;
    padding: 12px 24px;
    border: none;
    border-radius: 6px;
    cursor: pointer;
    width: 100%;
    margin-top: 20px;
    font-size: 16px;
    font-weight: 600;
    transition: background-color 0.2s;
}

.btn-primary:hover {
    background-color: #45a049;
}

.btn-primary:active {
    transform: scale(0.98);
}
```

---

### 任务 3: 修改 `frontend/script.js`

#### 3.1 添加控制变量
**位置**: `data()` 函数中

**在现有变量后添加**:
```javascript
data() {
    return {
        // ... 现有变量 ...
        courseEditOpen: false, // 控制课程编辑侧边栏开关
    }
}
```

#### 3.2 修复拼写错误（关键 BUG）
**位置**: `CourseSet()` 方法（约 155 行）和 `StudentSet()` 方法（约 169 行）

**修改前**:
```javascript
body: JSOM.stringify({
    id: this.setitem.id
}),
```

**修改后**:
```javascript
body: JSON.stringify({
    id: this.setitem.id
}),
```

#### 3.3 优化 `CourseSet()` 方法
**完整代码**:
```javascript
async CourseSet() {
    if (!this.setitem || !this.setitem.id) {
        alert('请先选择要修改的课程');
        return;
    }
    
    try {
        const res = await fetch(`/api/Set/Course?type=${this.setType}&q=${this.setQuery}`, {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify(this.setitem)
        });
        
        if (res.ok) {
            alert('修改成功！');
            this.courseEditOpen = false;
            // 刷新课程列表
            this.get_courses();
        } else {
            alert('修改失败，请稍后重试');
        }
    } catch (error) {
        console.error('提交失败:', error);
        alert('网络错误');
    }
}
```

---

## 测试检查清单

### 功能测试
- [ ] 点击课程卡片后，右侧侧边栏从右滑入
- [ ] 侧边栏显示当前课程的详细信息
- [ ] 所有输入框正确绑定到 `setitem` 的对应字段
- [ ] 点击"×"按钮，侧边栏关闭且 `setitem` 被清空
- [ ] 修改课程信息后点击"保存修改"，数据成功提交到后端
- [ ] 提交成功后侧边栏自动关闭

### 样式测试
- [ ] 侧边栏滑入动画流畅（0.35秒过渡）
- [ ] 表单布局整洁，间距合理
- [ ] 输入框获得焦点时有边框高亮效果
- [ ] 按钮 hover 效果正常
- [ ] 课程卡片鼠标悬停时显示指针光标

### 兼容性测试
- [ ] 学生详情侧边栏功能不受影响
- [ ] 两个侧边栏可以独立打开和关闭
- [ ] 不同分辨率下（1920x1080, 1366x768）侧边栏宽度合理

---

## 潜在风险与解决方案

### 风险 1: 侧边栏 z-index 冲突
**现象**: 如果同时打开学生和课程侧边栏，可能出现重叠
**解决**: 
- 在打开课程侧边栏时自动关闭学生侧边栏
- 修改点击事件: `@click="setitem = item; courseEditOpen = true; stuisOpen = false"`

### 风险 2: `setitem` 数据被意外修改
**现象**: 如果用户在侧边栏修改后未保存就关闭，原数据可能被污染
**解决**: 使用深拷贝
```javascript
@click="setitem = JSON.parse(JSON.stringify(item)); courseEditOpen = true"
```

### 风险 3: 后端 API 未实现或路径错误
**现象**: 提交时返回 404
**解决**: 
- 先在浏览器 Network 面板确认 `/api/Set/Course` 是否正常响应
- 检查 C++ 的 `CROW_ROUTE` 定义

---

## 实施顺序建议
1. **先修改 `script.js`** (修复 BUG，添加变量)
2. **再修改 `style.css`** (确保样式就绪)
3. **最后修改 `index.html`** (添加功能)

这样可以避免 HTML 引用不存在的变量或样式。

---

## 完成标准
- 所有文件修改完成且无语法错误
- 浏览器控制台无报错
- 点击课程卡片后侧边栏正常显示
- 修改数据并提交后，后端日志显示收到请求

---

**计划创建时间**: 2026-01-19  
**预计实施时间**: 15-20 分钟
