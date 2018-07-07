#include <napi.h>
#include <windows.h>
#include <stdio.h>
#include <psapi.h>
#include <assert.h>

namespace proc_info
{

PROCESS_MEMORY_COUNTERS_EX get_mem_info(DWORD pid)
{
  HANDLE proc;
  PROCESS_MEMORY_COUNTERS_EX mem_counters;

  proc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, false, pid);
  GetProcessMemoryInfo(proc, (PROCESS_MEMORY_COUNTERS *)&mem_counters, sizeof(mem_counters));
  CloseHandle(proc);

  return mem_counters;
}

void add_prop(napi_env env, napi_value output, char key[], SIZE_T value)
{
  napi_status status;

  napi_value prop_key;
  status = napi_create_string_utf8(env, key, strlen(key), &prop_key);
  assert(status == napi_ok);

  napi_value prop_value;
  status = napi_create_double(env, (double)value, &prop_value);
  assert(status == napi_ok);

  status = napi_set_property(env, output, prop_key, prop_value);
  assert(status == napi_ok);
}

napi_value get_proc_info(napi_env env, napi_callback_info info)
{
  napi_status status;
  size_t argc = 2;
  napi_value args[1];
  double pid;
  napi_value mem_info_obj;
  PROCESS_MEMORY_COUNTERS_EX mem_info;

  status = napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);
  assert(status == napi_ok);

  status = napi_get_value_double(env, args[0], &pid);
  assert(status == napi_ok);

  mem_info = get_mem_info((DWORD)pid);

  status = napi_create_object(env, &mem_info_obj);
  assert(status == napi_ok);

  add_prop(env, mem_info_obj, "pageFaultCount", mem_info.PageFaultCount);
  add_prop(env, mem_info_obj, "pagefileUsage", mem_info.PagefileUsage);
  add_prop(env, mem_info_obj, "peakPagefileUsage", mem_info.PeakPagefileUsage);
  add_prop(env, mem_info_obj, "peakWorkingSetSize", mem_info.PeakWorkingSetSize);
  add_prop(env, mem_info_obj, "privateUsage", mem_info.PrivateUsage);
  add_prop(env, mem_info_obj, "quotaNonPagedPoolUsage", mem_info.QuotaNonPagedPoolUsage);
  add_prop(env, mem_info_obj, "quotaPagedPoolUsage", mem_info.QuotaPagedPoolUsage);
  add_prop(env, mem_info_obj, "quotaPeakNonPagedPoolUsage", mem_info.QuotaPeakNonPagedPoolUsage);
  add_prop(env, mem_info_obj, "quotaPeakPagedPoolUsage", mem_info.QuotaPeakPagedPoolUsage);
  add_prop(env, mem_info_obj, "workingSetSize", mem_info.WorkingSetSize);

  return mem_info_obj;
}

napi_value init(napi_env env, napi_value exports)
{
  napi_status status;
  napi_value fn;

  status = napi_create_function(env, nullptr, 0, get_proc_info, nullptr, &fn);
  assert(status == napi_ok);

  status = napi_set_named_property(env, exports, "proc_info", fn);
  assert(status == napi_ok);

  return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, init)
} // namespace proc_info