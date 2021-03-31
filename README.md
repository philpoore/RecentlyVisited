# Recently Visited Service

A simple Recently Visited Service written in c++.

For each user, stores the last 100 recently visisted products / pages.

For each visit, the unix timestamp in nanoseconds of the visit is stored.

A Linked Hash Map is used for Least Recently Used data structure, giving following time complexity:
    - lookup Hash user visited product? O(1)
    - get list recently visited products O(1)
    - visit product O(1)

Only the last 100 unique products are stored. If a user revisits a product in the LRU, it timestamp is updated and 
moved to the beggining of the list.

Communication with service is done via tcp protocol.
The following binary commands are available:

### `SET`
Sets a recently visited product for a given user_id.
- user_id's can be any string
- product_id's are 64 bit unsigned ints
```
SET {user_id} {product_id}
```
Example:

Set user `123123` visiting product `5555`.
```
echo "SET 123123 5555" | nc 127.0.0.1 8091
# ok
```

### `GET`
Gets the recent visited list for a given user.
```
GET {user_id}
```

Example:
```
echo "GET 123123" | nc 127.0.0.1 8091
# 5555
```

## Build
```
make
```

## Run
Will start the server on port 8091.

```
./RecentlyVisited
```