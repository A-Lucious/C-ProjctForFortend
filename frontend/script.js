const { createApp } = Vue;

createApp({
    // 1. 数据中心：这里定义的变量，HTML 里可以直接用
    data() {
        return {
            history: [], // 存储 C++ 返回的 vector
            isOpen: true,

            searchType: "class_name",
            currentLabel: "搜索课程名",
            stuSearchType: "student_name",
            stuCurrentLabel: "搜索学生名",

            currentView: "calc", //当前导航栏物块
            searchQueryCourse: "",

            courselist: [],
            studentLlist: []
        }
    },
    watch:{
        searchQueryCourse(newVal,oldVal) {
            console.log('SearchQueryChange from ${oldVal} to ${newVal}');
            CourseSort();
        }
    },
    // 2. 方法中心：存放原本的 calculate 逻辑
    //async异步处理
    methods: {
        async refreshChart() {
            const response = await fetch('/get_history');
            const json = await response.json();
            this.history = json.data;

            // 调用 ECharts 画图
            const myChart = echarts.init(document.getElementById('chart-main'));
            myChart.setOption({
                xAxis: { type: 'category', data: this.history.map((_, i) => i + 1) },
                yAxis: { type: 'value' },
                series: [{ data: this.history, type: 'line', smooth: true, color: '#42b983' }]
            });
        },

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
            const response = await fetch(`/api/Search/Student?type=${this.stuSearchType}&q=${encodeURIComponent(this.stuCurrentLabel)}`);
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

        // 用户选择了某一项
        selectType(typeValue, labelText) {
            this.searchType = typeValue;   // 更新发给后端的参数
            this.currentLabel = labelText; // 更新按钮显示的字
            this.isOpen = false;           // 选完自动收起菜单
        },

        async CourseSort() {
            try {
                const response = await fetch('/api/Sort/Course',{
                    method: 'POST',
                    headers: { 'Content-Type': 'application/json' },
                    body: JSON.stringify(this.searchQueryCourse),
                });

                const data = await response.json(); 

                this.courselist = data;
            }catch(error) {
                console.error("排序失败",error);
            }
        },

        //拉去课程表
        async fetchCourses() {
            try {
                const response = await fetch('/api/get_courses');
                const data = await response.json();

                this.courselist = data;
            }catch(error) {
                console.error("获取信息失败: ",error);
            }
        }
    },
    // 3. 生命周期：网页加载完自动执行
    mounted() {
        this.refreshChart();
        this.fetchCourses();
    }
}).mount('#app');