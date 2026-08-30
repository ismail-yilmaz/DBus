# DBus Package for U++

**DBus** is a native D-Bus Inter-Process Communication (IPC) library for the [U++](https://www.ultimatepp.org/) framework.  This repository contains the core DBus components required to build both clients and servers entirely using native U++ code.

## Features

-   **Native Protocol Implementation:** Speaks the D-Bus binary protocol directly using standard U++ `Socket` and `String` classes, avoiding external dependencies like `libdbus` or `glib`.
    
-   **Event Loop Integration:** Integrates easily with U++'s `SocketWaitEvent` for asynchronous messaging. It sleeps when idle and wakes on network traffic, ensuring it won't burn CPU cycles or freeze your GUI.
    
-   **Full Client & Server Support:** Query properties, call remote methods, claim well-known bus names, listen for incoming requests, and send structured replies seamlessly.
    
-   **Built-in Authentication:** Automatically handles standard D-Bus `EXTERNAL` handshakes to connect cleanly to both local session buses and system buses.
    

## Safety Architecture

Writing IPC code in event-driven Linux apps usually leads to tricky bugs. This engine uses strict boundaries to prevent common crashes and deadlocks out of the box.

-   **Compile-Time Type Safety:** The `DBusValue` container makes sure you only pack valid D-Bus wire types (like `int16`, `int32`, `String`, arrays, or dictionaries). If you accidentally try to pass an incompatible U++ object (like a `Color` or `Font`), the code simply won't compile, saving you from random runtime crashes.
    
-   **Deadlock Prevention (Reentrancy Guard):** Calling a synchronous method from inside an asynchronous callback is a classic way to permanently freeze a D-Bus app. The engine uses an internal `EventLock` latch to detect this. If it happens, it safely aborts the illegal call and logs an error instead of hanging your program.
    
-   **Predictable Routing:** Guarantees messages are processed in the exact order they arrive by keeping the raw socket reading loop strictly separated from your custom callback code.

## Shared Data Types

DBus package uses a set of strict, move-aware variant containers to map standard C++ data directly into complex D-Bus wire signatures:
*   **DBusValue:** The core variant wrapper (supports standard primitives, strings, and nesting).
*   **DBusValueArray:** Standard arrays (automatically translates to `a*` signatures).
*   **DBusValueMap:** Key-value dictionaries (automatically translates to `a{sv}` signatures).
*   **DBusValueStruct:** D-Bus data structures (automatically translates to `(is)` signatures).

## Documentation & Tutorials

*   **Design Docs:** Pseudoblocking, Async Mechanics, and API Usage
*   **Tutorials:**
    *   Method Calling
    *   Complex Variant Marshaling
    *   Global Multiplexing and Async Event Loops
    *   Sending Desktop Notifications
    *   Building a Server Service

## Examples

The DBus nest ships with ready-to-run interactive examples demonstrating both client-side and server-side setups, from basic blocking calls to background daemons.

| Example | Description |
| :--- | :--- |
| **Method** | A basic example demonstrating D-Bus method calls. |
| **Notify** | A minimal client application demonstrating how to trigger a native desktop notification using a simple blocking method call. |
| **Monitor** | An asynchronous listener that subscribes to system-wide D-Bus broadcast signals and logs them to the console in real-time. |
| **Marshall** | Demonstrates how to marshall complex, poymorphic D-Bus data easily. |
| **Properties** | Demonstrates how to query and parse complex nested structures using the standard `org.freedesktop.DBus.Properties` interface. |
| **Server** | A headless background daemon that claims a well-known bus name and routes remote method requests asynchronously. |

