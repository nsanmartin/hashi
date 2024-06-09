#ifndef __MACS_HASHI_H__
#define __MACS_HASHI_H__

#define _hashi_concat(a, b) a##b
#define _hashi_concat2(a, b) _hashi_concat(a, b)
#define _hashi_cat(prefix, name) _hashi_concat2(prefix, _hashi_concat2(_, name))
#define _hashi_cat_(prefix, name) _hashi_concat2(_, _hashi_concat2(prefix, _hashi_concat2(_, name)))

#endif
