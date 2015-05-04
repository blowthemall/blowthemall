# Match server protocol

The match server is the server responsible to orchestrate the game match. It's
the server that knows the BlowThemAll game rules. It's the most important
server.

## Guidance

There are some concerns to be tackled in future versions of the protocol:

- Anti-bot.
  - We want to make as little semantic available to the clients as possible.
  - We want to randomize much about the stream to increase entropy.
- Verified P2P tournaments? (yeah, it sounds stupid).
- Create a responsive and fluid game enablement protocol. This value should be
  employed on LAN-based compos.
  - Things like position prediction.
  - This value may require greater game semantics sharing. It's the opposite of
    the anti-bot value.

## Overview

The game server runs a WebSocket server that will accept any connection. On top
of the WebSocket connections, there's a JSON-RPC 2.0 layer that exposes
BTANodes.

The default port for unencrypted connections is 8799. The default port fo
encrypted connections is 8800.

## BTANodes

BTANodes are our abstractions to communicating channels for each client-server
connection. The list of acceptable JSON-RPC methods for each node depends on
node's current state.

The node isn't required to share the same state with each connection.

### Server methods

#### `identification` state

This is the server's initial state

In this state, the BTANode will notify clients of their cookies, which can be
used to enter the game in case of disconnection. The notification happens by
calling the clients' `setCookie(string)` remote procedure. The server changes to
the `matchSetup` state right after.

#### `matchSetup` state

Available server's methods:

- `confirm()`: The player is ready to play and won't do any other change.
- `bool abort()`: The player changed his mind and wants to do more changes.

Once all clients call the `confirm` method, the server will change to the
`match` state.

#### `match` state

Available server's methods:

- `moveUp(secs: double)`: Ask to move the player's character in the up
  direction.
  - `secs`: The number of seconds the player hold the `up` key. Useful to group
    several `moveUp` commands into one and decrease network traffic.
- `moveRight(secs: double)`: Ask to move the player's character in the right
  direction.
  - `secs`: The number of seconds the player hold the `right` key. Useful to
    group several `moveRight` commands into one and decrease network traffic.
- `moveDown(secs: double)`: Ask to move the player's character in the down
  direction.
  - `secs`: The number of seconds the player hold the `down` key. Useful to
    group several `moveDown` commands into one and decrease network traffic.
- `moveLeft(secs: double)`: Ask to move the player's character in the left
  direction.
  - `secs`: The number of seconds the player hold the `left` key. Useful to
    group several `moveLeft` commands into one and decrease network traffic.
- `dropBomb`: Instruct player's intent to put a bomb now, if possible.

### Client methods

#### `identification` state

This is the client's initial state.

Available client's methods:

- `setCookie(string)`: Set the client's session id. In future versions of the
  protocol, it'll be useful to handle reconnections. After this method is
  called, the client changes to `matchSetup` state.

#### `matchSetup` state

Available client's methods:

- `matchStarted()`: Just a notification. The client changes to `match` state
  right after.

#### `match` state

- `itemAdded(id: string, x: double, y: double, z: double, animationSet: string, animationState: string)`.
- `itemAnimated(id: string, newAnimationState: string)`.
- `itemMoved(id: string, newX: double, newY: double, newZ: double, secs: double)`:
  Move the item on the map.
  - `secs`: The amount of time the animation should take. The client is free to
    use ping estimatives to predict accurate position.
- `winner(id: string)`: Used to notify the match's winner.
