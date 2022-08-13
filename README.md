# AVL

## ¿Cómo configurar Git Hooks?

```bash
# añadirle el permiso de ejecución al shell script
chmod +x ./hooks/configure.sh
# ejecutar el script
./hooks/configure.sh
```

## ¿Cómo correrlo?

### Logs Off

```bash
make prod
./avl
```

### Logs On

```bash
make debug
./avl
```

### Tests

```bash
make tests
./avltest
```

## Operaciones soportadas

|     Operación      | Complejidad |                                                                               Descripción                                                                               |                                                   Notas                                                   |
| :----------------: | :---------: | :---------------------------------------------------------------------------------------------------------------------------------------------------------------------: | :-------------------------------------------------------------------------------------------------------: |
|     `findKey`      |  `O(lg n)`  |                                             Devuelve el `key` (puntero) si lo encontró, de lo contrario devuelve `nullptr`                                              |
| `iterativeFindKey` |  `O(lg n)`  |                                                Misma funcionalidad que `findKey`, pero implementado de manera iterativa                                                 |                                                     -                                                     |
|       `find`       |  `O(lg n)`  |                            Devuelve el `value` (puntero) asociado al `key` si lo encontró, de lo contrario devuelve `nullptr`. Es recursivo.                            |                                                     -                                                     |
|      `insert`      |  `O(lg n)`  |                                                                      Inserta un `key`-`value` pair                                                                      |
| `iterativeInsert`  |  `O(lg n)`  |                                                   Misma funcionalidad que `insert`, pero inserta de manera iterativa                                                    |                                                     -                                                     |
|      `remove`      |  `O(lg n)`  |                                                                      Remueve un `key`-`value` pair                                                                      | Optimizado para que reemplace con el predecesor o sucesor dependiendo de cuál subárbol tiene menor altura |
|     `maximum`      |  `O(lg n)`  |       Devuelve una tupla con el `key` (de máximo valor) y su `value` correspondiente (ambos punteros). Si no hay `maximum`, devuelve una tupla con dos `nullptr`        |                                                     -                                                     |
|     `minimum`      |  `O(lg n)`  |       Devuelve una tupla con el `key` (de mínimo valor) y su `value` correspondiente (ambos punteros). Si no hay `minimum`, devuelve una tupla con dos `nullptr`        |                                                     -                                                     |
|   `predecessor`    |  `O(lg n)`  | Devuelve una tupla con el `key` y su `value` correspondiente (ambos punteros) del predecesor del `key` pasado como parámetro, sino devuelve una tupla con dos `nullptr` |                                                     -                                                     |
|     `sucessor`     |  `O(lg n)`  |  Devuelve una tupla con el `key` y su `value` correspondiente (ambos punteros) del sucesor del `key` pasado como parámetro, sino devuelve una tupla con dos `nullptr`   |                                                     -                                                     |
|  `inorderString`   |   `O(n)`    |                                                Retorna un _string_ que tiene todos los _keys_ del AVL en modo _inorder_                                                 |                                                     -                                                     |
|     `inorder`      |   `O(n)`    |                        Recorre el AVL de manera _inorder_ y recibe un _lambda_ como parámetro para procesar de alguna manera cada `key`-`value`                         |                                                     -                                                     |
|     `getRoot`      |   `O(1)`    |                                                                       Retorna el valor de la raíz                                                                       |                                                     -                                                     |
|      `clear`       |   `O(n)`    |                                                                  Borra todos los nodos en _postorder_                                                                   |                                                     -                                                     |
|    `getHeight`     |   `O(1)`    |                                                                       Devuelve la altura del AVL                                                                        |                                                     -                                                     |
