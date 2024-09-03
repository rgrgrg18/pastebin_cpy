## Docker scripts descriptions


----

* **create.sh** 

> Builds app image and pulls images of other services.


* **up.sh** 

> Creates containers and runs them. 
As second argument you can pass `no-logs`(runs all services without logs services),
`release` or nothing(runs all services), `postgres`(runs only postgres),
`redis`(runs only redis), `app`(runs only pastebin).


* **login.sh** 

> Opens app container console.


* **down.sh** 

> Stops containers and removes containers, networks.


* **start.sh**  

> Starts existing containers.
As second argument you can pass name of the service and only it will start.

* **stop.sh** 

> Stops running containers without removing them.
As second argument you can pass name of the service and only it will stop.


* **restart.sh** 

> Restarts all stopped and running containers.
As second argument you can pass name of the service and only it will restart.