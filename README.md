# liBDSM

A small project of SMB protocol client implementation in C, with a _lot_ less
features than Samba but also a more permissive license (WTFL/MIT). The initial
goal of this project is to have a lib with an iOS compatible license to be
intgrated into VLC for iOS.

The lib is provided as a static library or as a dynamically linked library. A
few useless (yet) utils are also provided.

## Building

### Requirements

* A Unix system with a bash-shell (i guess)
* C99 C compiler
* (GNU) Make
* Autotools
* libc with iconv
* getopt_long

Most of it can be installed on Debian(-based) systems using

    sudo apt-get install build-essential

### HowTo

    $> ./bootstrap
    $> ./configure --prefix=/your/ass
    $> make
    $> make install # maybe

## Goals

Here's a list of features i intend to support in this project :
* NETBIOS
  * Basic bi-directionnal NETBIOS name resolution [Done]
  * Hacky LAN SMB servers discovery (Listing all the smb servers on the LAN, no WINS, etc.) [Done]
  * Basic NETBIOS Session transport layer [Done]
* SMB
  * Support only required parts of 'NT LM 0.12' (aka CIFS?) dialect.
  * User based authentication [Done]
  * List Shares [Partial/Ugly]
  * Browse folders [Done/Ugly]
  * Read file [Done, can be improved :)]
  * No write, lock, RPCs, etc. [Hum]

## Support

liBDSM has been tested/reported to work witht the following devices/OSes:

* Windows 7
* Windows 8
* A cheap NAS whose name i can't remember :)
* Guest Login on French Orange 'Livebox' (more testing is welcome as i don't have the box password)

OSX is not supported yet, since it doesn't seem to support Netbios transport.
DirectTCP SMB is on the way, to fix this issue.

Feel free to contribute items to this list (or network trace of not working devices)

## Contributing

* Fork elthariel/libdsm
* Make a feature branch
* Commits your work there
* Make a pull request
* ...
* Profit !
