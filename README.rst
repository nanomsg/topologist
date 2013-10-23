==========
Topologist
==========

A topology configuration service for nanomsg_ and nanoconfig_.

.. _nanomsg: http://nanomsg.org
.. _nanoconfig: http://github.com/nanomsg/nanoconfig


Building
========

Dependencies:
* cmake (building only)
* nanomsg

For building from git::

    git submodule update --init --recursive
    mkdir build
    cd build
    cmake ..
    make
    sudo make install
