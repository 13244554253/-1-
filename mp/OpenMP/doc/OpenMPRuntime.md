# OpenMP 运行时库函数

#### 常用运行时库函数表
|        函数名      |               作用             |
|-------------------|--------------------------------|
|opm_set_num_threads|设置将在下一个并行区域中使用的线程数|
|opm_get_num_threads|返回当前在团队中执行并行区域的线程数,该区域时调用线程的地方|
|opm_get_max_threads|返回可通过调用opm_get_num_threads函数返回的最大值|
|opm_get_thread_num|返回在团队中执行此调用的线程的线程号|
|opm_get_thread_limit|返回程序中可用的OpenMP线程的最大数量|
|opm_get_num_procs|返回程序可用的处理器数量|
|opm_in_parallel|用于确定正在执行的代码段是否并行|
|opm_set_dynamic|用于确定是否启用动态线程调整|
|opm_set_nested|用于启用或禁用嵌套并行性|
|opm_get_nested|用于确定是否启用嵌套并行性|
|opm_set_schedule|在OpenMP指令中将"runtime"用作调度类型时,设置循环调度策略|
|opm_get_schedule|在OpenMP指令中使用"runtime"作为调度类型时,返回循环调度策略|
|opm_set_max-active_levels|设置嵌套并行区域的最大数目|
|opm_get_max-active_levels|返回嵌套并行区域的最大数目|
|opm_get_level|返回嵌套并行区域的当前级别|
|opm_get_ancestor_thread_num|对于当前线程的给定嵌套级别,返回祖先线程的线程数|
|opm_get_team_size|对于当前线程的给定嵌套级别,返回线程团队的大小|
|opm_get_active_level|返回包含调用的任务的嵌套活动并行区域的数目|
|opm_in_final|如果程序在最后一个任务区域执行,则返回true;否则返回false|
|opm_init_lock|初始化与锁变量相关的锁|
|opm_destory_lock|将给定的锁变量与锁分离|
|opm_set_lock|获得锁的所有权|
|opm_unset_lock|释放锁|
|opm_test_lock|尝试设置锁,但如果锁不可用，则不会阻塞|
|opm_init_nest_lock|初始化与锁变量关联的嵌套锁|
|opm_destory_nest_lock|将给定的嵌套锁变量与任何锁分离|
|opm_set_nest_lock|获取嵌套锁的所有权|
|opm_unset_nest_lock|释放嵌套锁|
|opm_test_nest_lock|尝试设置嵌套锁,但如果锁不可用,则不会阻塞|
|opm_get_wtime|提供便携式挂钟定时程序|
|opm_get_wtick|返回一个双精度浮点值，该值等于连续时钟滴答之间的秒数|
