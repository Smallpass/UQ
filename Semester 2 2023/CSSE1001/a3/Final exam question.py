Python 3.11.6 (tags/v3.11.6:8b6ee5b, Oct  2 2023, 14:57:12) [MSC v.1935 64 bit (AMD64)] on win32
Type "help", "copyright", "credits" or "license()" for more information.


>>> def foo(sx, sy):
...     substring = []
...     for i in enumerate(sy):
...         for k in range(len(sy)):
...             factor = sy.split[i-k,i]+sy.split[i,i+k]
...             substring.append(factor)
        return sx.split() in substring