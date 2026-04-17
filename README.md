# Badge Tropicon 2026

Badge Oficial de Trpicon 2026

## Hardware
- ESP32-C6
- AHT30
- SI4463
- VGdd79S915N0S1
- Wide Angle 2.79 inch 142x428 Color IPS TFT Display NV3007 Controller

## Tecnologia
- LoRa
- WIFi
- BLE
- Zigbee
- Thread
- [AIS](https://es.wikipedia.org/wiki/Sistema_de_Identificaci%C3%B3n_Autom%C3%A1tica)
- [meshtastic](https://github.com/meshtastic)

<div align="center" markdown="1">


<h1>Tropicon 2026 — Firmware</h1>

Firmware personalizado de Meshtastic para el badge del evento **Tropicon 2026**, desarrollado por **Electronic Cats**. Incluye integración de agenda de pláticas, talleres, monitor AIS de embarcaciones, telemetría ambiental y mensajería LoRa.

</div>


## Primeros pasos
> [!IMPORTANT]
> INSTRUCCIONES COMPLETAS DE USO

Dirigete al [Repositorio de Fimrware](https://github.com/ElectronicCats/Tropicon-2026_Firmware) para instrucciones completas de uso

## Botones y navegación general

El badge cuenta con cuatro botones físicos:

| Botón | Pulsación corta | Pulsación larga (~1 s) |
|-------|----------------|------------------------|
| **UP** | Pantalla anterior / Ítem anterior | Abrir menú de acciones de la pantalla actual |
| **DOWN** | Pantalla siguiente / Ítem siguiente | — |
| **LEFT** | Izquierda / Día anterior (en Talks) | — |
| **RIGHT** | Derecha / Siguiente stage (en Talks) | — |

> [!TIP]
> El **botón UP largo** es la acción principal de cada pantalla. Desde cualquier pantalla, úsalo para acceder a las opciones disponibles: enviar mensajes, navegar a la agenda, configurar LoRa, etc.

---

## Pantallas del dispositivo

Navega entre pantallas con **UP** (anterior) y **DOWN** (siguiente). El ciclo completo es:

| # | Pantalla | Descripción |
|---|----------|-------------|
| 1 | **Talks** | Agenda de pláticas y talleres de Tropicon 2026 |
| 2 | **AIS** | Monitor de embarcaciones (Automatic Identification System) |
| 3 | **Clock** | Reloj digital o analógico |
| 4 | **Home** | Estado del nodo: nombre, batería y canal LoRa |
| 5 | **Messages** | Mensajes de texto recibidos y enviados en la malla LoRa |
| 6 | **Node List** | Lista de nodos detectados en la malla |
| 7 | **Telemetry** | Métricas del entorno: Temperatura y Humedad (sensor AHT10) |
| 8 | **GPS** | Brújula y coordenadas (deshabilitado — sin módulo GPS) |
| 9 | **LoRa** | Estadísticas de radio: SNR, RSSI y frecuencia |
| 10 | **System** | Información de sistema: uptime y memoria libre |

---

## Menú de Acciones (Home Action)

Desde la pantalla **Home**, presiona **UP largo** para abrir el menú *"Home Action"*:

| Opción | Qué hace |
|--------|----------|
| **Back** | Cierra el menú sin cambios |
| **Sleep Screen** | Apaga la pantalla para ahorrar batería (cualquier botón la reactiva) |
| **Talks** | Abre directamente la agenda de pláticas y talleres |

---

## Agenda de Pláticas y Talleres (Talks)

La agenda carga automáticamente los datos desde la flash del dispositivo:

| Archivo | Contenido |
|---------|-----------|
| `/schedule.json` | Pláticas principales (Track 1 y Track Villas) |
| `/talleres.json` | Talleres simultáneos (Salón Maya, La Isla, Suite 1, Suite 2) |

Para acceder a la agenda puedes:
- Navegar con **UP/DOWN** hasta llegar a la pantalla **Talks**, o
- Desde cualquier pantalla, abrir el menú con **UP largo** y seleccionar **Talks**.

### Vista lista

Muestra el listado de pláticas del día y stage (escenario) actualmente seleccionados. El encabezado indica `Día — Stage`.

| Acción | Resultado |
|--------|-----------|
| **UP** corto | Sube al ítem anterior de la lista |
| **UP** corto (en la primera plática) | Sale de la agenda y regresa al menú principal |
| **DOWN** | Baja al ítem siguiente de la lista |
| **LEFT** | Cambia al día anterior (cíclico); resetea el stage y la selección |
| **RIGHT** | Avanza al siguiente stage; si ya es el último stage, avanza al siguiente día |
| **UP largo** | Entra a la **vista detalle** de la plática seleccionada |

> [!TIP]
> Para **salir de la agenda en cualquier momento**, presiona **UP** cuando estés en la primera plática de la lista — el badge regresará automáticamente al menú principal de pantallas.

### Vista detalle

Muestra el título completo, nombre del ponente, horario, sala, imagen del ponente y el marcador de interés actual.

| Acción | Resultado |
|--------|-----------|
| **UP** corto | Regresa a la vista lista |
| **DOWN** | Avanza a la siguiente plática (permanece en vista detalle) |
| **LEFT** | Retrocede a la plática anterior (cambia de stage/día si es necesario) |
| **RIGHT** | Avanza a la plática siguiente (cambia de stage/día si es necesario) |
| **UP largo** | Cicla el estado de interés: `Sin interés → Asistir → Tal vez → Saltar` |

### Marcadores de interés y notificaciones

Los marcadores se muestran en la esquina superior derecha de cada plática en la vista lista:

| Marcador | Estado | Descripción |
|----------|--------|-------------|
| _(vacío)_ | Sin interés | Sin acción |
| `[A]` | **Asistir** | El badge enviará una notificación a tu app Meshtastic 10 minutos antes de que comience |
| `[T]` | **Tal vez** | Recordatorio opcional |
| `[S]` | **Saltar** | Marcado para no asistir |

> [!IMPORTANT]
> Los marcadores se guardan automáticamente en la flash del dispositivo y **persisten entre reinicios**. Si tienes el badge conectado a la app Meshtastic y una plática marcada como `[A]` (Asistir) está por comenzar, recibirás una notificación push en tu teléfono con el mensaje _"Tropicon: [nombre de plática] empieza en 10 min"_.

---

## Monitor AIS de Embarcaciones

La pantalla **AIS** recibe y decodifica señales VHF del sistema de identificación automática de embarcaciones (AIS), usado internacionalmente por barcos para reportar su posición y datos.

Para acceder, navega con **UP/DOWN** hasta la pantalla **AIS**.

### Información mostrada

La pantalla se divide en:

- **Encabezado**: muestra el canal AIS activo (`ch A` o `ch B`, es decir canal 87B o 88B) y el total de tramas recibidas (`frm`).
- **Lista de embarcaciones** (ordenadas de más reciente a más antigua): cada fila contiene:
  - **MMSI**: identificador único de la embarcación (9 dígitos)
  - **RSSI**: nivel de señal recibida en dBm
  - **Antigüedad**: tiempo desde la última trama recibida (en segundos, o `>1h`)
  - **Tipo de mensaje**: badge `T1`, `T2`, etc. (tipo de trama AIS)
  - **Sentencia NMEA**: frase AIS cruda recibida (truncada para caber en pantalla)

Cuando no se han recibido embarcaciones, la pantalla muestra `Listening...` indicando que el receptor está activo y esperando señales.

> [!NOTE]
> El AIS trabaja en frecuencias VHF (~162 MHz). La recepción depende de la proximidad a cuerpos de agua navegables y de la línea de visión con las embarcaciones.

---

## Mensajes — Recibir y Enviar

La pantalla **Messages** muestra todos los mensajes de texto intercambiados en la malla LoRa, presentados en formato de burbuja (mensajes propios a la derecha, mensajes recibidos a la izquierda).

### Navegar a la pantalla de mensajes

Usa **RIGHT/LEFT** desde cualquier pantalla hasta llegar a **Messages**.

### Ver mensajes

- Cada mensaje muestra: remitente, tiempo transcurrido, canal o DM, y el texto.
- Los mensajes propios muestran un indicador de estado de entrega:
  - Palomita `✓` — mensaje confirmado (ACK del destino)
  - `✗` — falla o tiempo de espera agotado
  - Círculo — reenviado por un nodo relay

### Enviar un mensaje (nuevo)

1. Navega a la pantalla **Messages**.
2. Presiona **UP largo** para abrir el menú *"Message Action"*.
3. Selecciona **Reply** → **With Preset** para elegir un mensaje predefinido de la lista.
4. Usa **RIGHT/LEFT** para seleccionar el mensaje deseado y **UP largo** para enviarlo.

> [!NOTE]
> La opción **With Freetext** (teclado libre) solo aparece si se detecta un teclado externo conectado al badge.

### Responder a un mensaje recibido

1. Navega a la pantalla **Messages** donde aparece el mensaje.
2. Presiona **UP largo** para abrir *"Message Action"*.
3. Selecciona **Reply** y elige:
   - **With Preset** — selecciona una respuesta de la lista de mensajes predefinidos.
   - **With Freetext** — escribe tu propio texto (requiere teclado externo).

### Otras opciones del menú Message Action

| Opción | Qué hace |
|--------|----------|
| **Back** | Cierra el menú |
| **Reply** | Abre el submenú para responder |
| **View Chats** | Cambia el filtro de vista: todos los mensajes, por canal o por conversación directa |
| **Mute Channel** / **Unmute Channel** | Silencia o reactiva las notificaciones del canal actual |
| **Delete** | Elimina mensajes (el más antiguo, el actual o todos) |

---

## Telemetría del Entorno

La pantalla **Telemetry** muestra las métricas ambientales medidas en tiempo real por el sensor **AHT10** integrado en el badge:

- **Temperatura** en grados Celsius
- **Humedad relativa** en porcentaje (%)

Para acceder, navega con **UP/DOWN** hasta llegar a la pantalla **Telemetry**.

Las métricas se actualizan automáticamente cada segundo. El badge también transmite estos datos periódicamente por la malla LoRa para que otros nodos y la app Meshtastic puedan visualizarlos.

---

### Recursos útiles

- [Documentación oficial de Meshtastic](https://meshtastic.org/docs/) — arquitectura, módulos, protocolo LoRa y API.
- [Repositorio base de Meshtastic firmware](https://github.com/meshtastic/firmware) — código fuente upstream del que se deriva este firmware.
- [Documentación de PlatformIO](https://docs.platformio.org/) — comandos de compilación, ambientes y gestión de librerías.
- [Foro de la comunidad Meshtastic](https://meshtastic.discourse.group/) — preguntas, proyectos y soporte comunitario.
- [Electronic Cats en GitHub](https://github.com/ElectronicCats) — otros proyectos de hardware y firmware de Electronic Cats.


## Credits

Designed and developed by the **Electronic Cats** firmware and hardware team.
