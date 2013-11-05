====================
Topologist Reference
====================

Topologist configuration is a YAML_ file, that is parsed using quire_. So
you might refer to the documentation of both for more expret info.

.. _quire: http://quire.readthedocs.org
.. _YAML: http://yaml.org


Sections
========

Layouts
-------

The ``layouts`` section describes the layouts in form of mapping. Layouts are
reusable parts of the topology. Layouts represented in graph-like structure.


Topologies
----------

The ``topologies`` section is a directory of the real topologies that might
be resolved by end client. There are currently two kinds of topologies:

* normal topology (the ``!Topology`` tag) is represented by hirarchy of
  layouts and define the connections between nodes of the topology
* extern topology (the ``!Extern`` tag) is used to "extract" some public
  node addresses from other normal topollogy. So that clients, that connect
  to the topology never connect to some internal addresses regardless of what
  role and other parameters they specify.

Tips And Tricks
===============

Including files
---------------

You can put every layout and every topology into it's own file and include
them in the following way:

.. code-block:: yaml

   layouts: !GlobMap "./layouts/(*).yaml"
   topologies: !GlobMap "./topologies/(*).yaml"

Note the following things:

* The name of the layout and topology is derived from the name of the
  file, in particular the part that is matched inside parenthesis.
* The path to file is relative to the directory where original configuration
  file is, not relative to the current working directory as one may expect

