## Project scripts description


----

* **build.sh** 

> build project files into `build`, in `build/app/` we have `pastebin_server` - out project executable file, 
in `build/test/` we have all tests bin files (in this script we generate proto files)


* **destroy.sh** 

> remove `build` dir


* **rebuild.sh** 

> run `destroy.sh` and `build.sh`


* **test.sh**  

> run `build.sh` (if build dir doesn't exist) and ctest in build dir


* **run.sh** 

> run `test.sh` and `build/app/pastebin_server`