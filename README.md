# libshamuneko

! this is still in development and this README could be polished...

This is a small and simple comic/manga/manhua/whatever scraping
library.

libshamuneko functionality is implemented via 'Services', which are
 small lua modules. These modules are nothing more than recipes for
 scraping sites.

libshamuneko is flexible. It isn't strictly bound to any HTTP library
 by default, so instead, you wire up your own callbacks and hook it
 in. On the Lua side, it exposes simple libxml2 and libpcre2
 bindings. It doesn't expose anything deemed unsafe or unneccesary.

# Why is the name so long?

Sorry

# License

Licensed under the BSD 2-Clause license.
