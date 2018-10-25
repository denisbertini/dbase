FairDB
========

The **FairDB** Database interface is on Object Relational Mapping library
(ORM) which implements a time based version management for parameter handling.


## Installation

You need to first install the FairRoot framework and then follow the
instructions at the
[FairRoot github page.](https://github.com/FairRootGroup/FairRoot)

## Getting started

Tutorials how to write/read parameter classes are provided in the
[dbExamples](https://github.com/denisbertini/dbase/tree/dev/dbExamples) subdirectory.

* [Basics](https://github.com/denisbertini/dbase/tree/dev/dbExamples/basics)
shows how to use the FairDB interface to read and write objects using the generic parameter interface to any supported
database system i.e MySQL, PosGreSQL and SQLite.
Two type of parameter classes are implemented with
simple data members [FairDbTutPar](https://github.com/denisbertini/dbase/blob/dev/dbExamples/basics/src/FairDbTutPar.h)
complex data members i.e ROOT object [FairDbTutParBin](https://github.com/denisbertini/dbase/blob/dev/dbExamples/basics/src/FairDbTutPar.h)

* [Advanced](https://github.com/denisbertini/dbase/tree/dev/dbExamples/advanced)
  shows how to use more advanced features like dedicated validity intervals, composite objects and caching mechanism.

In both cases, corresponding macro to read and write as well as a script. To setup the database to be used (dbconfig.sh) are available from the macro directory


## FairDB Web Interface
FairDB uses the powerful [Wt framework](htpp://www.webtoolkit.eu/wt) to allow the user to
quickly develop web application.
The following picture  shows the **Cbm Sts Web Interface** as  an example of the web programming functionality using FairDB.
![Cbm Sts Web Interface](/doc/stsqaweb.png)

### Availability
The **FairDB Web Interface** has been sucessfuly tested on the latest **MacOSx (Mevericks, Yosemite)** as well as the latest **Ubuntu Linux ( 14.04LTS and 15.04)**.
If you experience some problem with other system just email me @ <d.bertini@gsi.de>

### Installation
In order to compile the FairDB interface code, you will need to first install the Wt library.
You can find general install documentation [here](http://http://redmine.emweb.be/projects/1/wiki/Installing_Wt_on_Ubuntu).
But be aware that the install procedure has to be slighlty adapted in order to work properly with
FairDB.

* The **BOOST** library used in the *FairSoft* should be also used to compile Wt.
* The [**Haru** free PDF library](http:://libharu.org) has to be installed and used to compile Wt. You can
find instruction how to install the **Haru** library [here](http:://libharu.org).



* Install script tested for Ubuntu (14.04 LTS and 15.04):
```bash
cmake -DHARU_PREFIX=/usr/local/lib -DBOOST_DIR=<where_FairSoft_is_installed> <path_to_Wt_source>
```

* Install script tested for MacOSX (Mavericks, Yosemite):
```bash
cmake -DHARU_PREFIX=/usr/local/lib -DBOOST_DIR=<Where_FairSoft_Is_Installed>
-DCMAKE_CXX_FLAGS='-stdlib=libc++' -DCMAKE_EXE_LINKER_FLAGS='-stdlib=libc++'
-DCMAKE_OSX_ARCHITECTURES=x86_64 -DCMAKE_MODULE_LINKER_FLAGS='-stdlib=libc++'
-DWT_CPP_11_MODE='-std=c++11' -DPOSTGRES_PREFIX='/usr/local/pgsql'
-DMYSQL_PREFIX='/usr/local/mysql' -DMYSQL_LIBRARY='/usr/local/mysql/lib' <path_to_wt_source>
```

And just install the library and the Wt examples in your system:

```bash
> cd build
> make
> make install
> make -C examples
```

## Install FairDB with Wt
Once Wt is installed on your system, you can install FairDB by adding and building
the **dbase** module in **FairRoot**.
For that you need to get **dev** branch of the dbase module from its
[GitHub repository ](https://github.com/denisbertini/dbase) i.e:

```bash
> cd <path_to_FairRoot>
> git clone https://github.com/denisbertini/dbase dbase
> cd dbase
> git fetch
> git checkout -b dev origin/dev
```

Check if you are using the FairDB source from the
development branch

```bash
> git branch -a
```

In order to compile FairRoot with FairDB you should set the cmake flag
-DWITH_DBASE=ON to generate the corresponding Makefiles.


## Runing the Cbm Sts QA Web Interface.

The FairDB source directories are the following:

```bash
-rw-r--r--   1 denis  staff   4.3K Aug 26 15:03 CMakeLists.txt
-rw-r--r--   1 denis  staff   4.9K Aug 26 15:03 FairDBLinkDef.h
-rw-r--r--   1 denis  staff   3.6K Sep  1 12:43 README.md
drwxr-xr-x   3 denis  staff   102B Aug 26 15:03 cmake/
drwxr-xr-x   4 denis  staff   136B Aug 26 15:03 dbExamples/
drwxr-xr-x   8 denis  staff   272B Apr  2 10:54 dbIO/
drwxr-xr-x   4 denis  staff   136B Aug 26 15:03 dbInput/
drwxr-xr-x  90 denis  staff   3.0K Aug 26 15:03 dbInterface/
drwxr-xr-x   6 denis  staff   204B Apr  2 10:54 dbMQ/
drwxr-xr-x  17 denis  staff   578B Apr  2 10:54 dbUtils/
drwxr-xr-x  14 denis  staff   476B Aug 26 15:03 dbValidation/
drwxr-xr-x  21 denis  staff   714B Aug 31 12:28 dbWt/
drwxr-xr-x   3 denis  staff   102B Sep  1 11:03 doc/
```

The **FairDB-Wt** base classes  can be found in the **/dbWt** directory.
The Cbm Sts QA Web interface classes can be found in the **dbExamples/cbm_sts** directory:

```bash
-rw-r--r--   1 denis  staff    87B Aug 26 15:03 CMakeLists.txt
drwxr-xr-x   5 denis  staff   170B Aug 28 13:20 data/
drwxr-xr-x   8 denis  staff   272B Aug 31 12:42 macros/
drwxr-xr-x  13 denis  staff   442B Aug 31 11:47 src/
drwxr-xr-x  32 denis  staff   1.1K Sep  1 11:40 web/
```

### Priming the Database
You need first to prepare the database in order to run the example.
In the **/src** directory one can found the macros to prime the database with some example
Sts sensor Qa data-files.
```bash
> cd macros
```
Configure the back-end database where you want to store the data as tables. This example
suppose that you  already installed MySQL back-end and configure it which means that
you have created a database (cbm),  a user (scott) and the corresponding password (ySqaoRT1234).

```bash
> . dbconfig.sh local_mysql
local_mysql  session configured as:
FAIRDB_TSQL_PSWD=ySqaoRT1234
FAIRDB_TSQL_URL=mysql://localhost/cbm
FAIRDB_TSQL_USER=scott
```
The run the  macros to prime the DB.
```bash
> root stsqa_prime.C -l
 ```

### Running the HTTP server and the web application
To run the web application just move to the FairRoot *build/bin* directory where all FairRoot
executables are located.

```bash
> cd build/bin
> ./cbmstsqawt.wt
-I- FAIRDbConnectionPool  fGlobalSeqNoDbNo  0
-I- ~ Creating ~ FairDbParFactory
-I- CbmStsQaWt::getWorkEnd()
            Working environement is -----> /Users/denis/fairdb/fairroot/fairbase/
[2015-Sep-01 13:20:25.914509] 44090 - [info] "config: reading Wt config file:
           /Users/denis/fairdb/fairroot/fairbase/dbase/dbWt/approot/wt_config.xml
           (location = './cbmstsqawt.wt')"
[2015-Sep-01 13:20:25.915375] 44090 - [info] "WServer/wthttp: initializing built-in wthttpd"
[2015-Sep-01 13:20:25.915826] 44090 - [info] "wthttp: started server: http://127.0.0.1:5564"
 ```
The HTTP server is now ready to listen to any client on the localhost loopback address
(127.0.0.1) at port 5564.
Go to your favourite browser and type simply the address and the port to connect
your HTTP server as a client.

The treeview on the left side shows the list of parameter objects stored
in the SQL database backend. To fetch the data from the database and view the parameter
contents, select first the parameter you want to visualize. A mouse right-click shows a dropdown
menu. Select *"View"* to visualize :

![Cbm Sts Web Interface](/doc/cbmstsqa_init.png)

To view the Current-Voltage curve for a particular sensor, select it and again mouse right click
and use the menu *"showIV"* :

![Cbm Sts Web Interface](/doc/cbmstsqa_view.png)


### Customizing your HTTP server

The setup for the HTTP server can be changed to your need
by editing the source file which generate the process at:

```bash
> cd <path_to_FairRoot>/dbase/dbExamples/cbm_sts/web
> emacs cbmstsqawt.cxx &
 ```
At line 151, on can change the port and address for the HTPP server in the argument
list:

```bash
 const char * const v[9] =
  {
    argv[0],
	"--docroot", str_doc.c_str(),
	"--approot", str_app.c_str(),
    "--http-port", "5564",
    "--http-address", "127.0.0.1"
  };
 ```
You need then to recompile and re-run the wt process.

### Authors

FairDB is a rewrite/re-design of the original database interface code from Nick West for the Minos
collaboration at Fermilab.

New extensions have been implemented (Denis Bertini, Evgeny Lavrik):
* Including support for Oracle, PosGresSQL and SQLite database backend.
* Generic template based data classes using CRTP design allowing automatic generation of C++ code
* IO interface library to FairROOT RuntimeDB interface
* Wt interface library allowing data scraping/visualization from DB to any WebBrowser
                               using HTTP protocol
* ZeroMQ based multiple client access for scalability
