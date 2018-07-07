# node-win-proc-info

**Node native addon for the [`GetProcessMemoryInfo`](https://docs.microsoft.com/en-us/windows/desktop/api/psapi/nf-psapi-getprocessmemoryinfo) function of the Windows [Process Status API](https://docs.microsoft.com/en-us/windows/desktop/api/_psapi/).**

## Installation

```sh
yarn add win-proc-info
```

## Quickstart

```js
const procInfo = require('win-proc-info')

console.log(procInfo(process.pid))
/* { pageFaultCount: 6178,
     pagefileUsage: 10211328,
     peakPagefileUsage: 10690560,
     peakWorkingSetSize: 21942272,
     privateUsage: 10211328,
     quotaNonPagedPoolUsage: 12960,
     quotaPagedPoolUsage: 164960,
     quotaPeakNonPagedPoolUsage: 12960,
     quotaPeakPagedPoolUsage: 164960,
     workingSetSize: 21942272 } */
```

## License

MIT
