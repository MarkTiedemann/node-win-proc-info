#include <napi.h>
#include <windows.h>
#include <psapi.h>

namespace proc_info
{

void add_prop(napi_env env, napi_value output, char key[], SIZE_T value)
{
  napi_value prop_value;
  napi_create_double(env, (double)value, &prop_value);
  napi_set_named_property(env, output, key, prop_value);
}

napi_value get_proc_info(napi_env env, napi_callback_info info)
{
  size_t argc = 1;
  napi_value args[1];
  napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);

  double pid;
  napi_get_value_double(env, args[0], &pid);

  auto proc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, false, (DWORD)pid);

  PROCESS_MEMORY_COUNTERS_EX mem_counters;
  GetProcessMemoryInfo(proc, (PROCESS_MEMORY_COUNTERS *)&mem_counters, sizeof(mem_counters));

  CloseHandle(proc);

  napi_value proc_info;
  napi_create_object(env, &proc_info);

  add_prop(env, proc_info, "pageFaultCount", mem_counters.PageFaultCount);
  add_prop(env, proc_info, "pagefileUsage", mem_counters.PagefileUsage);
  add_prop(env, proc_info, "peakPagefileUsage", mem_counters.PeakPagefileUsage);
  add_prop(env, proc_info, "peakWorkingSetSize", mem_counters.PeakWorkingSetSize);
  add_prop(env, proc_info, "privateUsage", mem_counters.PrivateUsage);
  add_prop(env, proc_info, "quotaNonPagedPoolUsage", mem_counters.QuotaNonPagedPoolUsage);
  add_prop(env, proc_info, "quotaPagedPoolUsage", mem_counters.QuotaPagedPoolUsage);
  add_prop(env, proc_info, "quotaPeakNonPagedPoolUsage", mem_counters.QuotaPeakNonPagedPoolUsage);
  add_prop(env, proc_info, "quotaPeakPagedPoolUsage", mem_counters.QuotaPeakPagedPoolUsage);
  add_prop(env, proc_info, "workingSetSize", mem_counters.WorkingSetSize);

  return proc_info;
}

napi_value init(napi_env env, napi_value exports)
{
  napi_value fn;
  napi_create_function(env, nullptr, 0, get_proc_info, nullptr, &fn);
  napi_set_named_property(env, exports, "get_proc_info", fn);
  return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, init)
} // namespace proc_info