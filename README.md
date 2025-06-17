# Ray Traced Sphere Renderer with OpenGL Quad

This project uses **OpenGL** to render a fullscreen quad (two triangles), and applies a dynamically generated texture created using **raytracing**.

### 🌀 How It Works

- **OpenGL Quad**: A quad made of two triangles fills the entire window.
- **Raytracing**: Each pixel on the quad is computed by shooting a ray into a 3D scene from a virtual camera.
- **Scene**: Currently includes spheres, with support for adding more objects.
- **Lighting**: Basic directional lighting is applied using Lambertian reflection (dot product between normal and light direction).
- **Texture Mapping**: The result of the raytracer is converted into a texture and rendered in real-time.

### ✅ Features

- Ray-sphere intersection detection
- Directional light shading
- Basic material coloring
- Spheres can move (vertical bobbing)
- Uses OpenGL to efficiently display the final image

### 📦 Dependencies

- [GLFW](https://www.glfw.org/)
- [GLEW](http://glew.sourceforge.net/)
- [GLM](https://github.com/g-truc/glm)

**Note:** All dependencies are already included in the repository — just run:

```bash
make
./raytracer
