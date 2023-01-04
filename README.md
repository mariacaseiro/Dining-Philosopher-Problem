# Dining Philosopher Problem using semaphores. 2 versions: processes and threads
Author: María Caseiro Arias


### Solution in C with POSIX semaphores, 2 versions: one with processes and other with threads.
### Solución en C con semáforos, 2 versiones: una con procesos y otra con hilos.

-------------------------------------------------------------
En el código adjuntado se presenta una solución al problema de los **filósofos comilones**, presentado en 1965 por **Edsger Dijkstra** para representar el problema de la sincronización de procesos en un sistema operativo.
Cabe destacar que la interpretación está basada en pensadores chinos, quienes comían con dos palillos, donde es más lógico que se necesite el del comensal que se siente al lado para poder comer.

### El problema a implementar es el siguiente:
Cinco filósofos se sientan alrededor de una mesa y pasan su vida cenando y pensando. Cada filósofo tiene un plato de fideos y un tenedor a la izquierda de su plato. Para comer los fideos son necesarios dos tenedores y cada filósofo sólo puede tomar los que están a su izquierda y derecha. Si cualquier filósofo toma un tenedor y el otro está ocupado, se quedará esperando, con el tenedor en la mano, hasta que pueda tomar el otro tenedor, para luego empezar a comer.
Si dos filósofos adyacentes intentan tomar el mismo tenedor a la vez, se produce una condición de carrera: ambos compiten por tomar el mismo tenedor, y uno de ellos se queda sin comer.
Si todos los filósofos toman el tenedor que está a su derecha al mismo tiempo, entonces todos se quedarán esperando eternamente, porque alguien debe liberar el tenedor que les falta. Nadie lo hará porque todos se encuentran en la misma situación (esperando que alguno deje sus tenedores). Entonces los filósofos se morirán de hambre. Este bloqueo mutuo se denomina **interbloqueo o deadlock**.

El problema consiste en encontrar un algoritmo que permita que los filósofos nunca se mueran de hambre. 
Esta solución está basada en el uso de **semáforos**. Además, se incluyen dos implementaciones, una utilizando procesos y otra usando hilos.

-------------------------------------------------------------
The attached code presents a solution to the problem of **gluttonous philosophers**, presented in 1965 by **Edsger Dijkstra** to represent the problem of process synchronization in an operating system. The interpretation is based on Chinese thinkers, who ate with two chopsticks, where it is more logical that the diner who sits next to him is needed to eat.

### Problem to implement:
Five philosophers sit around a table and spend their lives dining and thinking. Each philosopher has a plate of noodles and a fork to the left of his plate. To eat the noodles you need two forks and each philosopher can only take the ones to his left and right. If either philosopher takes a fork and the other is busy, he will wait until he can take the other fork, and then start eating. If two adjacent philosophers attempt to take the same fork at the same time they both compete to take the same fork and one of them is left without eating.
If two adjacent philosophers attempt to take the same fork at the same time they both compete to take the same fork and one of them is left without eating.
If all the philosophers take the fork that is to their right at the same time, then they'll all be waiting forever, because someone must release the fork that is missing. Nobody will do it because everyone is in the same situation (waiting for someone to put down their forks). Then the philosophers will starve. This deadlock is called **deadlock**.
