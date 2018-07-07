declare function procInfo(
  pid: number
): {
  pageFaultCount: number
  pagefileUsage: number
  peakPagefileUsage: number
  peakWorkingSetSize: number
  privateUsage: number
  quotaNonPagedPoolUsage: number
  quotaPagedPoolUsage: number
  quotaPeakNonPagedPoolUsage: number
  quotaPeakPagedPoolUsage: number
  workingSetSize: number
}

export = procInfo
