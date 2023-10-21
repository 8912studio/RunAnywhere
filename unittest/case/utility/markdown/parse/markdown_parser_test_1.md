# Hex command help

Display content of file in hex byte format.

**Usage**

```
hex [text] [position] [length] /f /u8 /u16
```

`position` starts with `` ` ``. `length` starts with `~`, followed by a number.

**Switches**
* `/f`

    Treats `text` as file path.

* `/u8`

    Treats active path as string and display it in UTF-8 encoding.

* `/u16`

    Treats active path as string and display it in UTF-16 encoding.

Example:
```
hex `32 ~16
hex "string to display" /u8
```