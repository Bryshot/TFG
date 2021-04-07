#Formato del archivo JSON para importar/exportar la configuración del plugin.
El formato de fichero necesario para realizar una correcta importación de la configuración del plugin, así como el del fichero resultante al exportar la configuración del plugin; es la siguiente:

* El archivo comenzara con una llave de apertura “{” y terminara con una llave de cierre “}”; entre estos caracteres, separados por comas, pueden aparecer los siguientes elementos:
  * contestName: String, es el nombre del torneo.
  * contestServerWebsite: String, dirección web de la api del Domjudge.
  * cycleSize: Integer, duración del ciclo de visionado.
  * passwordContestServer: String, contraseña de administrador en el api del Domjudge.
  * userContestServer: String, usuario administrador en el api del Domjudge.
  * delayIP: Integer, tiempo de espera en ms antes de hacer efectivo un cambio de VLC. 
  * delayJugdement: Integer, tiempo en ms entre comprobaciones de nuevos veredictos. 
  * interval: Integer, tiempo de espera en ms entre ciclos del plugin.
  * numPendingWeight: Float, peso en la heurística del número de envíos pendientes.
  * rankWeight: Float, peso en la heurística de la posición en la clasificación.
  * timeInStreamWeight: Float, peso en la heurística del número de apariciones previas.
  * speedRotativeText: Integer, velocidad de rotación del texto móvil.
  * startHotkey: Array, conjunto de teclas que ejercen de hotkey de inicio del plugin.
  * stopHotkey: Array, conjunto de teclas que ejercen de hotkey de inicio del plugin.
  * toggleHotkey: Array, conjunto de teclas que ejercen de hotkey de inicio del plugin.
  * threadPriority: Integer [0-6], la prioridad que tienen los QThread que forman el plugin.
  * verbose: Booleano, si se debe mostrar información de debug por consola.
  * generalTabPos: Integer [0-2], posición que ocupa en el gui la pestaña General.
  * heuristicTabPos: Integer [0-2], posición que ocupa en el gui la pestaña Heurística.
  * timerTabPos: Integer [0-2], posición que ocupa en el gui la pestaña Timers.
