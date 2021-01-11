----
WolkAbout C++11 SDK for implementing Wolkabout connector, gateway or gateway module for connecting devices to [WolkAbout IoT Platform](https://demo.wolkabout.com/#/login).

Supported protocol(s):
* WolkAbout protocol

Prerequisite
------
Following tools/libraries are required in order to build WolkAbout C++ connector

* cmake - version 3.5 or later
* autotools
* autoconf
* m4
* zlib1g-dev
* libssl-dev


Former can be installed on Debian based system from terminal by invoking

`sudo apt-get install autotools-dev autoconf m4 zlib1g-dev cmake libssl-dev`

SDK is consisted of three parts: Connectivity, Poco and Gtest which can be configured with cmake flags BUILD_CONNECTIVITY, BUILD_POCO and BUILD_GTEST recpectivly.

* Connectivity
Includes Wolkabout protocol implementation and paho mqtt library.

* Poco
Includes poco library with sql and ssl utilities.

* Gtest
Includes gtest and gmock libraries for writing unit tets.
