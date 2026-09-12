# DBus Nest for U++

This repository contains a complete, native D-Bus Inter-Process Communication (IPC) suite for the [U++](https://www.ultimatepp.org/) framework. It consists of two primary packages: **DBus** (the core binary protocol implementation) and **DBusLib** (a modern, fluent API for standard desktop interfaces). Both are built entirely using native U++ code.

## Features

* **Cross-Platform Support:** Works on both POSIX-based systems and Windows natively, using unix (filesystem, abstract) and tcp sockets.
* **Native Protocol Implementation:** Speaks the D-Bus binary protocol directly using standard U++ `Socket` and `String` classes, avoiding external dependencies like `libdbus` or `glib`.
* **High-Level Fluent API (DBusLib):** Provides object-oriented wrappers for common desktop standards (MPRIS Media Players, NetworkManager, Desktop Notifications, XDG Portals, Introspection) using clean builder patterns and safe payload extraction.
* **Event Loop Integration:** Integrates easily with U++'s `SocketWaitEvent` for asynchronous messaging. It sleeps when idle and wakes on network traffic, ensuring it won't burn CPU cycles or freeze your GUI.
* **Full Client & Server Support:** Query properties, call remote methods, claim well-known bus names, listen for incoming requests, and send structured replies seamlessly.
* **Built-in Authentication:** Automatically handles standard D-Bus `EXTERNAL` handshakes to connect cleanly to both local session buses and system buses.

## Documentation & Tutorials

* **Design Docs:** Pseudoblocking, Async Mechanics, API Usage, and DBusLib Architecture
* **Tutorials:**
* Method Calling
* Complex Variant Marshaling
* Global Multiplexing and Async Event Loops
* Sending Desktop Notifications (via DBusLib)
* Building a Server Service



## Examples

The DBus nest ships with ready-to-run interactive examples demonstrating both client-side and server-side setups, from basic blocking calls to background daemons.

| Example | Description |
| --- | --- |
| **FileChooser** | Demonstrates D-Bus native file chooser integration using desktop portal. |
| **Introspection** | Demonstrates how to dynamically discover an object's methods, signals, and properties using `DBusInspector`. |
| **Marshall** | Demonstrates how to marshall complex, polymorphic D-Bus data easily. |
| **Method** | A basic example demonstrating D-Bus method calls. |
| **Monitor** | An asynchronous listener that subscribes to system-wide D-Bus broadcast signals and logs them to the console in real-time. |
| **Mpris** | Demonstrates discovering, querying, and controlling desktop media players natively using `DBusMediaPlayer`. |
| **NetworkManager** | Demonstrates the basic usage of NetworkManager's D-Bus interface. |
| **Notify** | A minimal client application demonstrating how to trigger a native desktop notification using a simple blocking method call. |
| **Properties** | Demonstrates how to query and parse complex nested structures using the standard `org.freedesktop.DBus.Properties` interface. |
| **Server** | A headless background daemon that claims a well-known bus name and routes remote method requests asynchronously. |