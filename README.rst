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
    cmake .. -DCMAKE_INSTALL_PREFIX=/usr
    make
    sudo make install
