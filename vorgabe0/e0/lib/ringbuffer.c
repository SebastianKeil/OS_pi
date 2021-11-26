/*
************* from: https://www.mikrocontroller.net/articles/FIFO#Code-Beispiel****************

fifo_init(): FIFO initialisieren
fifo_write_busy(): wartet bis mindesten 1 Element im FIFO frei ist und schreibt es in den FIFO
fifo_write(): schreibt ein Element in den FIFO, ohne Prüfung ob genügend Platz vorhanden ist
fifo_read_busy(): wartet bis mindesten 1 Element im FIFO vorhanden ist und liest es aus dem FIFO
fifo_read(): liest ein Element es aus dem FIFO, ohne Prüfung ob Daten vorhanden sind
fifo_get_level(): liefert die Anzahl der Elemente im FIFO zurück
fifo_get_free(): liefert die Anzahl der noch frei verfügbaren Elemente im FIFO zurück
*/

#include <kernel/kprintf>

#define BUFFER_FAIL     0
#define BUFFER_SUCCESS  1
#define BUFFER_SIZE 23

struct Buffer {
  unsigned char data[BUFFER_SIZE];
  unsigned int read; // zeigt auf das Feld mit dem ältesten Inhalt
  unsigned int write; // zeigt immer auf leeres Feld
} 
buffer = {{}, 0, 0};

unsigned int BufferIn(uint8_t byte){
  //if (buffer.write >= BUFFER_SIZE)
  //  buffer.write = 0; // erhöht sicherheit

  if ( ( buffer.write + 1 == buffer.read ) ||
       ( buffer.read == 0 && buffer.write + 1 == BUFFER_SIZE ) )
    return BUFFER_FAIL; // voll

  buffer.data[buffer.write] = byte;

  buffer.write++;
  if (buffer.write >= BUFFER_SIZE)
    buffer.write = 0;

  return BUFFER_SUCCESS;
}

//
// Holt 1 Byte aus dem Ringbuffer, sofern mindestens eines abholbereit ist
//
// Returns:
//     BUFFER_FAIL       der Ringbuffer ist leer. Es kann kein Byte geliefert werden.
//     BUFFER_SUCCESS    1 Byte wurde geliefert
//    
uint8_t BufferOut(uint8_t *pByte)
{
  if (buffer.read == buffer.write)
    return BUFFER_FAIL;

  *pByte = buffer.data[buffer.read];

  buffer.read++;
  if (buffer.read >= BUFFER_SIZE)
    buffer.read = 0;

  return BUFFER_SUCCESS;
}
