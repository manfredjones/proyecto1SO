# Referencias de Schedulers

Este folder contiene versiones base de schedulers (`RoundRobin`, `Lottery`, `RealTime`)
usando `pthread`, que sirvieron como guía para diseñar los schedulers en `mypthreads`.

Aunque no se integran directamente con `mypthreads` (que usa `ucontext`), su lógica
podría adaptarse fácilmente a `scheduler_next()` para agregar múltiples políticas.
