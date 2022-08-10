# AVL

## Operaciones soportadas

|           Operación            | Complejidad |                                                   Descripción                                                    |                                                   Notas                                                   |
| :----------------------------: | :---------: | :--------------------------------------------------------------------------------------------------------------: | :-------------------------------------------------------------------------------------------------------: |
|           `findKey`            |  `O(lg n)`  |            Devuelve el valor del `key` si lo encontró, de lo contrario devuelve el macro `NOT_FOUND`             |                                                     -                                                     |
|            `insert`            |  `O(lg n)`  |                                                 Inserta un `key`                                                 |                                                     -                                                     |
|            `remove`            |  `O(lg n)`  |                                                 Remueve un `key`                                                 | Optimizado para que reemplace con el predecesor o sucesor dependiendo de cuál subárbol tiene menor altura |
|           `maximum`            |  `O(lg n)`  |                                        Devuelve el `key` de máximo valor                                         |                                                     -                                                     |
|           `minimum`            |  `O(lg n)`  |                                        Devuelve el `key` de mínimo valor                                         |                                                     -                                                     |
|         `predecessor`          |  `O(lg n)`  |                 Devuelve el predecesor de un `key` si existe, sino devuelve el macro `NOT_FOUND`                 |                                                     -                                                     |
|           `sucessor`           |  `O(lg n)`  |                  Devuelve el sucesor de un `key` si existe, sino devuelve el macro `NOT_FOUND`                   |                                                     -                                                     |
|        `inorderString`         |   `O(n)`    |                     Retorna un _string_ que tiene todos los _keys_ del AVL en modo _inorder_                     |                                                     -                                                     |
|           `inorder`            |   `O(n)`    | Recorre el AVL de manera _inorder_ y recibe un _lambda_ como parámetro para procesar de alguna manera cada `key` |                                                     -                                                     |
|           `getRoot`            |   `O(1)`    |                                           Retorna el valor de la raíz                                            |                                                     -                                                     |
| `getMaxWeightPathBetweenLeafs` |   `O(n)`    |         Obtiene el peso del camino de máximo peso entre dos hojas, funciona para cualquier árbol binario         |                                                     -                                                     |
|            `clear`             |   `O(n)`    |                                       Borra todos los nodos en _postorder_                                       |                                                     -                                                     |
|          `getHeight`           |   `O(1)`    |                                            Devuelve la altura del AVL                                            |                                                     -                                                     |

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
