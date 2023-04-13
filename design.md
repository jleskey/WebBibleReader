## Pipe format

Request pipe format:

```
version=version&reference=book:chapter:verse&num_verse=1
```

Example (Exodus 12:1):

```
version=kjv&reference=2:12:1&num_verse=1
```

Response pipe format:

```
status=STATUS&body=verse text
```

## Extra credit features

### Extra Bible versions

The other Bible versions mentioned in the instructions are accessible through a dropdown menu on the end-user interface. The lookup backend can handle providing each version.

### Shiny

It's also a little bit formatted, if that counts for anything!
