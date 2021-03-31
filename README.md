# Recently Visited Service

A simple Recently Visited Service written in c++.

For each user, stores the last 100 recently visisted products / pages.

For each visit, the unix timestamp in nanoseconds of the visit is stored.

A Linked Hash Map is used for Least Recently Used data structure, giving following time complexity:

    - get: list recently visited products for user O(1)
    - set: user visits product O(1)

Only the last 100 unique products are stored. If a user revisits a product in the LRU, it timestamp is updated and 
moved to the beggining of the list.

Communication with service is done via tcp protocol.
The following binary commands are available:

### `set`
Sets a recently visited product for a given user_id.
- user_id's can be any string
- product_id's are 64 bit unsigned ints
```
set {user_id} {product_id}
```
Example:

Set user `123123` visiting product `5555`.
```
echo "set 123123 5555" | nc 127.0.0.1 8091 -q0
# ok
```

### `get`
Gets the recent visited list for a given user.
```
get {user_id}
```

Example:
```
echo "get 123123" | nc 127.0.0.1 8091
# 5555
```

### `quit`
Terminate the connection.

## Build
```
make
```

## Run
Will start the server on port 8091.

```
./RecentlyVisited
```
