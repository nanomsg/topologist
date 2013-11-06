===================
Topologist Tutorial
===================

Topologist configuration is a YAML_ file, that is parsed using quire_. So
you might refer to the documentation of both for more expret info.


.. _quire: http://quire.readthedocs.org
.. _YAML: http://yaml.org


Simple Example
==============

Here is a simple configuration file for one service:

.. code-block:: yaml

    server:
      config-addr: [tcp://10.0.0.1:10001]
      updates-addr: [tcp://10.0.0.2:10002]

    layouts:
      simple:
        server <- client:
          port: 10010

    topologies:
      simple: !Topology
        type: reqrep
        layout: simple
        assign:
          server:
          - host: 10.0.0.3

In consists of the following sections.

The Server Section
------------------

.. code-block:: yaml

    server:
      config-addr: [tcp://10.0.0.1:10001]
      updates-addr: [tcp://10.0.0.2:10002]

It consists of addresses where to listen to for requests and some other
server-wide settings. The addresses in example are in square brackets because
they are actually YAML lists, for the case you want multiple addresses to
listen to (such as on different network interfaces).


The Layouts Section
-------------------

This section represents graph of the connections with connection settings.
Given the example:

.. code-block:: yaml

    layouts:
      simple:
        server <- client:
          port: 10010

There are two roles ``client`` and ``server``. Where server is bound on the
port ``10010`` and client connects to it.


The Topologies Section
----------------------

