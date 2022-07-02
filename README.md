# AVL

## Operaciones soportadas hasta ahora

- `inorder` (con [lambda](https://en.cppreference.com/w/cpp/language/lambda) como parámetro para procesar nodo)
- `clear` (borra todos los nodos en _postorder_)
- `insert`
- `getMaxWeightPathBetweenLeafs` (obtiene el peso del camino de máximo peso entre dos hojas, funciona para cualquier árbol binario)
- `successor`
- `findKey`

## TODO List

- [ ] `predecessor`
- [ ] `delete`
- [ ] volver a cada nodo genérico con un parámetro que sea un _lambda_ que compare dos nodos y retorne 1, 0 o -1 (dependiendo si uno es mayor, son iguales o el otro es mayor)
- [ ] corregir la mantenibilidad y legibilidad del código
- [ ] añadir atributo `parent` a los nodos para que `successor` y `predecessor` sean más simples
