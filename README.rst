==========
Topologist
==========

:Status: Pre-alpha
:Documentation: http://topologist.readthedocs.org/

A topology configuration service for nanomsg_ and nanoconfig_.

The project is meant to be reference implementation configuration service.
In other words, it should be able to define arbitrarily complex topology, and
leverage all the features of the nanonmsg. But for some situations it's
configuration format may be sub-optimal.


Building
========

Dependencies:
* cmake (build-time only)
* nanomsg

For building from git::

    git submodule update --init --recursive
    mkdir build
    cd build
    cmake .. -DCMAKE_INSTALL_PREFIX=/usr
    make
    sudo make install

.. _nanomsg: http://nanomsg.org
.. _nanoconfig: http://github.com/nanomsg/nanoconfig
