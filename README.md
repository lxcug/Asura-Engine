# Asura Game Engine

## 1. Composition of The Engine

1. Event System.
2. Use the conception of **Layer**, all the layers is placed in **LayerStack**, so all the layers will be rendered in order, so as to the order in which events are processed.

3. Encapsulation of Shader, Renderer, OpenGL Window etc.
4. Global Unique Identifier(GUID).
5. Log System(Bases on spdlog).
6. ECS System(based on entt).
7. Scene System(Include Scene, Camera, the serialize and deserialize of secne).
8. Editor(Based on ImGui and ImGuizmo).
9. Physical Engine(Based on box2d).

## 2. Demonstrate of Asura-Editor

**Asura-Editor**

<img src="https://raw.githubusercontent.com/lxcug/imgs/main/imgs20230108222603.png" style="zoom:50%;" />

Deserialize scene

<img src="https://raw.githubusercontent.com/lxcug/imgs/main/imgs20230108222647.png" style="zoom:50%;" />

Simulation

<img src="https://raw.githubusercontent.com/lxcug/imgs/main/imgs20230108222756.png" style="zoom:50%;" />