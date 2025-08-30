# Eraser Bot – Firmware ESP32  

Este repositorio contiene el código fuente para la **ESP32**, encargado de controlar los sensores y el sistema de borrado automático del **pizarrón inteligente Eraser Bot**.  

## Funcionalidad principal  

1. **Digitalización automática del contenido**  
   - Lectura de sensores de movimiento (UWB / ESP CAM) que rastrean el trayecto del marcador.  
   - Conversión del movimiento en datos que pueden enviarse a la aplicación.  

2. **Borrado automático**  
   - Control de un **motor paso a paso** mediante la ESP32.  
   - Activación del sistema de borrado desde un **botón físico** o **comando remoto vía Bluetooth/WiFi**.  
