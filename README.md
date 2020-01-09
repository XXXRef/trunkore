

<p align="center"> <h1 align="center">trunkore</h1> </p>
<p align="center"> <img align="center" src="https://xxxref.com/wp-content/uploads/2020/01/logo.png" width="759" height="130"> </p>
Super tool to manage concurrently running tasks


## Table of contents
- [What is it](#what-is-it)

- [How to build](#how-to-build)

- [How to use](#how-to-use)

## **What is it**
Want to do many things simultaneously, but have only two arms? Dont panic, **trunkore** is exactly the thing you need now!
Put your business logic in dynamic library and easily run it with other stuff concurrently, ofc with various abilities of state-managing such as initializing/suspending/deinitializing tasks.
And yeah, only in this universe, **trunkore** is not only free to use, but also opensourced. What a place to live, right? ;)

## **How to build**
**trunkore** uses CMake as build system
```
git clone https://github.com/XXXRef/trunkore.git
cd trunkore
mkdir BUILD
cd BUILD
cmake ..
```

**NOTES:**
1. For now only x86-Debug build configuration tested
2. You can build only **trunkore** without test job lib, just cmake the "trunkore" subfolder

## **How to use**
**trunkore** manages jobs that are contained each in its own job library. So you need to implement your own IJobOwner class (including i_jobowner.hpp, types.hpp, utils.hpp like in TestJobLib EXT/inc folder), and override {init(),play(),deinit()} methods with your business logic. Build result as dynamic lib and you are ready to go.

**Commands:**
1. Add job into **trunkore**
```
ADD <jobID> <jobLibPath> <jobConfigPath>
//Example: ADD job0 C:\job.dll C:\job_cfg.cfg
```
2. Remove job
```
RM <jobID>
```
3. Init job
```
INIT <jobID>
```
4. Deinit job
```
DEINIT <jobID>
```
5. Play job (for the first time or one being stopped)
```
PLAY <jobID>
```
6. Stop running job
```
STOP <jobID>
```
7. Get state of job
```
STATE <jobID>
```
8. List job IDs and their states
```
LS
```

**NOTES:**
1. < jobConfigPath > - path to config file with whatever additional info you want for your job to initialize itself
