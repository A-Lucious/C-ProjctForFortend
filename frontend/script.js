const { createApp } = Vue;

createApp({
    // 1. 数据中心：这里定义的变量，HTML 里可以直接用
    data() {
        return {
            history: [], // 存储 C++ 返回的 vector
            isOpen: false,
            isSortOpen: false,
            stuisOpen: false,

            selectStudent: null,
            currentStudent: null, // the student selected

            searchType: "fuzzy",
            currentLabel: "Fuzzy",

            stuSearchType: "fuzzy",
            stuCurrentLabel: "Fuzzy",

            currentView: "calc", //当前导航栏物块
            searchQueryCourse: "",
            searchQueryStudent: "",

            courselist: [],
            studentlist: [],

            charts: {
                grades: null,
                credits: null,
                gpa: null
            },

            currentSortLabel: "NetID",
            sortType: "fuzzy",

            //for set
            setType: "",
            setQuery: "",
            setitem: null,
            setpos: false,
            
            // 控制课程编辑侧边栏
            courseEditOpen: false,
            
            // 控制学生编辑侧边栏
            studentEditOpen: false,

            // Toast notifications
            toasts: [],
            toastIdCounter: 0,

            //fro import
            importType: null,

            //for export
            exportAddress: "",
        }
    },
    watch:{
        currentView(newVal,oldVal) {
            console.log(`View switching: ${oldVal} -> ${newVal}`);
            
            this.courseEditOpen = false;
            this.studentEditOpen = false;
            this.stuisOpen = false;
            
            this.isOpen = false;
            this.isSortOpen = false;
            
            this.setitem = null;
            this.currentStudent = null;

            if (newVal === 'calc') {
                this.fetchCourses();
            } else if (newVal === 'stus') {
                this.fetchStudents();
            }
        },
        sortType(newVal,oldVal) {
            if(newVal !== oldVal & newVal === "netid" || newVal === "student_number" || newVal === "student_name") {
                this.StudentSort();
            }else if (newVal !== oldVal & newVal === "course_teacher" || newVal === "course_number" || newVal === "course_name") {
                this.CourseSort();
            }
        },
    },
    // 2. 方法中心：存放原本的 calculate 逻辑
    //async异步处理
    methods: {

        async handleCourseSearch() {
            const response = await fetch(`/api/Search/Course?type=${this.searchType}&q=${encodeURIComponent(this.searchQueryCourse)}`);
            const data = await response.json();
            if (!data || (Array.isArray(data) && data.length === 0)) {
                alert("cant find")
            }else  {
                this.courselist = data;
            }

        },

        async handleStudentSearch() {
            const response = await fetch(`/api/Search/Student?type=${this.stuSearchType}&q=${encodeURIComponent(this.searchQueryStudent)}`);
            const data = await response.json();
            if (!data || (Array.isArray(data) && data.length === 0)) {
                alert("cant find")
            }else {
                this.studentlist = data;
            }
        },

        toggleDropdown() {
            this.isOpen = !this.isOpen;
        },

        toggleSortDropdown() {
            this.isSortOpen = !this.isSortOpen;
        },

        // 用户选择了某一项
        selectType(typeValue, labelText) {
            this.searchType = typeValue;   // 更新发给后端的参数
            this.currentLabel = labelText; // 更新按钮显示的字
            this.isOpen = false;           // 选完自动收起菜单
        },

        selectstuType(typeValue, labelText) {
            this.stuSearchType = typeValue;
            this.stuCurrentLabel = labelText;
            this.isOpen = false;
        },

        selectSortType(typeValue, labelText) {
            this.sortType = typeValue;
            this.currentSortLabel = labelText;
            this.isSortOpen = false;
        },

        async CourseSort() {
            try {
                const response = await fetch(`/api/Sort/Course?type=${this.sortType}`,{
                    method: 'POST',
                    headers: { 'Content-Type': 'application/json' },
                    body: JSON.stringify(this.courselist),
                });

                const data = await response.json(); 

                this.courselist = data;
            }catch(error) {
                console.error("排序失败",error);
            }
        },

        async StudentSort() {
            try {
                const response = await fetch(`/api/Sort/Student?type=${this.sortType}`,{
                    method: 'POST',
                    headers: {'Content-Type': 'application/json' },
                    body: JSON.stringify(this.studentlist),
                });

                const data = await response.json();

                this.studentlist = data;
                console.log(data);
            }
            catch(error) {
                console.error("排序失败",error);
            }
        },

        //set
        async CourseSet() {
            if (!this.setitem || !this.setitem.id) {
                alert('请先选择要修改的课程');
                return;
            }
            
            try {
                const response = await fetch(`/api/Set/Course`, {
                    method: 'POST',
                    headers: {'Content-Type': 'application/json' },
                    body: JSON.stringify({
                        item: this.setitem
                    }),
                })
                
                if (response.ok) {
                    alert('修改成功！');
                    this.courseEditOpen = false;
                    this.fetchCourses();
                } else {
                    alert('修改失败，请稍后重试');
                }
            }catch(error) {
                console.error("Set Error",error);
                alert('网络错误');
            }
        },

        async StudentSet() {
            if (!this.setitem || !this.setitem.stunum) {
                alert('请先选择要修改的学生');
                return;
            }
            
            try {
                const response = await fetch(`/api/Set/Student`, {
                    method: 'POST',
                    headers: {'Content-Type': 'application/json' },
                    body: JSON.stringify({
                        item: this.setitem
                    }),
                })
                
                if (response.ok) {
                    alert('修改成功！');
                    this.studentEditOpen = false;
                    this.fetchStudents();
                } else {
                    alert('修改失败，请稍后重试');
                }
            }catch(error) {
                console.error("Set Error",error);
                alert('网络错误');
            }
        },

        //拉去课程表
        async fetchCourses() {
            try {
                const response = await fetch('/api/get_courses');
                const data = await response.json();

                this.courselist = data;
                console.log(this.courselist)
            }catch(error) {
                console.error("获取信息失败: ",error);
            }
        },
        async fetchStudents() {
            try {
                const response = await fetch('/api/get_students');
                const data = await response.json();

                this.studentlist = data;
            }catch(error) {
                console.error("Get the students info failed",error);
            }
        },

        async fetchOneCourse() {
            try {
                const response = await fetch(`/api/get_onecourse?id=${this.setitem.id}`);
                const data = await response.json();

                this.setitem = data;
                console.log(this.setitem);
            }catch(error) {
                console.error("Fetch One Course failed",error);
            }
        },

        async fetchOneStudent() {
            try {
                const response = await fetch(`/api/get_onestudent?id=${this.setitem.stunum}`);
                const data = await response.json();

                this.setitem = data;
                console.log(this.setitem);
            }catch(error) {
                console.error("Fetch One Student fialed",error);
            }
        },

        closeDetail() {
            this.stuisOpen = false;
        },

        //import
        triggerImport(type) {
            this.importType = type;
            this.$refs.fileInput.click();
        },

        async handleImportFile(event) {
            const file = event.target.files[0];
            if (!file) return;
            const response = await fetch(`/api/import?type=${this.importType}&pos=${file.name}`);
            if (response.ok) {
                console.log(`Imported ${this.importType}`);
            }
            this.fetchCourses();
        },

        //export
        async exportCourses() {
            const response = await fetch(`/api/export_courses?pos=${this.exportAddress}.json`);
            alert("Exported Courses")
            console.log("Exported Courses");
            console.log(this.courselist);
        },

        async exportStudents() {
            const response = await fetch(`/api/export_students?pos=${this.exportAddress}.json`);
            alert("Exported Students");
            console.log("Exported Students");
            console.log(this.studentlist);
        },

        // 【核心修改】打开详情页方法
        // 假设你在调用这个方法之前，已经设置好了 this.currentStudent
        async openStudentDetail() {
            try {
                const response = await fetch('/api/get_currentStudent',{
                    method: "POST",
                    headers: { 'Content-Type': 'application/json' },
                    body: JSON.stringify({
                        value: this.selectStudent.stunum
                    }),
                });

                const data = await response.json();

                this.currentStudent = data;
                console.log(this.currentStudent)
            }catch(error) {
                console.error("Get the student info failed",error);
            }

            if (!this.currentStudent) {
                console.error("当前没有选中的学生 (currentStudent implies null)");
                return;
            }

            console.log("正在打开详情页，当前学生:", this.currentStudent.name);
            this.stuisOpen = true;

            // 等待侧边栏的 HTML 渲染出来后，再画图
            this.$nextTick(() => {
                this.initDetailCharts();
            });
        },

        // 【修改后】初始化图表，使用真实的 JSON 数据结构
        initDetailCharts() {
            // 1. 安全检查：确保 currentStudent 和 chart_data 存在
            const student = this.currentStudent;
            if (!student || !student.chart_data) {
                console.warn("缺少图表数据");
                return;
            }

            const chartData = student.chart_data;

            // ============================
            // 2. 数据解析 (Data Parsing)
            // ============================

            // --- A. 解析成绩数据 (用于条形图) ---
            // 提取所有课程名作为 X 轴
            const courseNames = chartData.grades_history.map(item => item.course_name);
            // 提取所有分数作为 Y 轴数据
            const scores = chartData.grades_history.map(item => item.score);

            // --- B. 解析学分数据 (用于左侧饼图) ---
            const totalCredit = chartData.credits.total_required;
            const earnedCredit = chartData.credits.earned;
            // 计算未修学分，防止出现负数
            const unearnedCredit = Math.max(0, totalCredit - earnedCredit);

            // --- C. 解析绩点数据 (用于右侧饼图) ---
            // 逻辑：计算每门课对学期总绩点的“贡献度” (学分 x 绩点)
            const gpaPieData = chartData.current_semester_gpa.map(item => ({
                value: (item.credit * item.gpa).toFixed(2), // 计算权重：学分 * 绩点
                name: item.course_name // 课程名
            }));


            // ============================
            // 3. 渲染图表 (ECharts SetOption)
            // ============================

            // --- 图表 1: 历史成绩统计 (条形图) ---
            const chartGradesDom = document.getElementById('chart-grades');
            if (this.charts.grades) this.charts.grades.dispose();
            this.charts.grades = echarts.init(chartGradesDom);
            
            this.charts.grades.setOption({
                title: { 
                    text: `${student.basic_info.name} 的成绩概览`, // 使用 basic_info 里的名字
                    left: 'center' 
                },
                tooltip: { trigger: 'axis' },
                grid: { left: '3%', right: '4%', bottom: '3%', containLabel: true }, // 防止标签溢出
                xAxis: { 
                    type: 'category', 
                    data: courseNames,
                    axisLabel: { interval: 0, rotate: 30 } // 课程名太长时倾斜显示
                },
                yAxis: { type: 'value', max: 100 },
                series: [{
                    data: scores,
                    type: 'bar',
                    itemStyle: { color: '#5470C6' },
                    label: { show: true, position: 'top' } // 柱子顶部显示具体分数
                }]
            });

            // --- 图表 2: 学分完成度 (饼图 - 左) ---
            const chartCreditsDom = document.getElementById('chart-credits');
            if (this.charts.credits) this.charts.credits.dispose();
            this.charts.credits = echarts.init(chartCreditsDom);

            this.charts.credits.setOption({
                title: { text: '学分修读情况', left: 'center', top: '5%' },
                tooltip: { 
                    trigger: 'item',
                    formatter: '{b}: {c} ({d}%)' // 显示：名称: 数值 (百分比)
                },
                legend: { bottom: '0%' },
                series: [{
                    name: '学分',
                    type: 'pie',
                    radius: ['40%', '70%'], // 环形
                    avoidLabelOverlap: false,
                    label: { show: false, position: 'center' },
                    emphasis: {
                        label: { 
                            show: true, 
                            fontSize: 16, 
                            fontWeight: 'bold',
                            formatter: '{b}\n{c}' // 中间高亮显示数值
                        }
                    },
                    data: [
                        { value: earnedCredit, name: '已修学分', itemStyle: { color: '#91cc75' } },
                        { value: unearnedCredit, name: '未修学分', itemStyle: { color: '#eee' } }
                    ]
                }]
            });

            // --- 图表 3: 本学期绩点权重 (饼图 - 右) ---
            const chartGpaDom = document.getElementById('chart-gpa');
            if (this.charts.gpa) this.charts.gpa.dispose();
            this.charts.gpa = echarts.init(chartGpaDom);

            this.charts.gpa.setOption({
                title: { text: '本学期绩点权重', left: 'center', top: '5%' },
                tooltip: { 
                    trigger: 'item',
                    formatter: function(params) {
                        // 自定义提示框：显示 课程名、学分、绩点
                        // params.data 包含了我们上面 map 出来的 value 和 name
                        // 但为了显示更多信息，我们可以在 map 时把原始 item 也放进去，或者这里简单显示 value
                        return `${params.name}<br/>贡献值: ${params.value}`;
                    }
                },
                legend: { type: 'scroll', bottom: '0%' }, // 课程多时可以滚动
                series: [{
                    name: '绩点贡献',
                    type: 'pie',
                    radius: '50%',
                    data: gpaPieData,
                    emphasis: {
                        itemStyle: {
                            shadowBlur: 10,
                            shadowOffsetX: 0,
                            shadowColor: 'rgba(0, 0, 0, 0.5)'
                        }
                    }
                }]
            });

            // 监听窗口大小变化
            window.addEventListener('resize', () => {
                this.charts.grades && this.charts.grades.resize();
                this.charts.credits && this.charts.credits.resize();
                this.charts.gpa && this.charts.gpa.resize();
            });
        }
        
    },
    // 3. 生命周期：网页加载完自动执行
    mounted() {
        this.fetchCourses();
        this.fetchStudents();
    }
}).mount('#app');