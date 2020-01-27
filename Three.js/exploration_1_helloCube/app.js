//Three.js code goes here
//Tutorial at https://threejsfundamentals.org/threejs/lessons/threejs-fundamentals.html

var renderer = new THREE.WebGLRenderer();
renderer.setSize( window.innerWidth, window.innerHeight );
document.body.appendChild( renderer.domElement );


const fov = 75; //Field of view
const aspect = 2;  // the canvas default
const near = 0.1; //Closer than this wont be shown
const far = 5; //Further than this wont be shown
const camera = new THREE.PerspectiveCamera(fov, aspect, near, far); //This defines the "frustum" which can be seen as a pyramid with a tip sliced off

camera.position.z = 2; //Moving the camera back from 0.0.0 to 0.0.2

const scene = new THREE.Scene();

//Creating a geometry
const boxWidth = 1;
const boxHeight = 1;
const boxDepth = 1;
const geometry = new THREE.BoxGeometry(boxWidth, boxHeight, boxDepth);

//Creating a material. MeshPhongMaterial can be affected by light
const material = new THREE.MeshPhongMaterial({color: 0x44aa88});  // greenish blue

//Light
{
  const color = 0xFFFFFF;
  const intensity = 1;
  const light = new THREE.DirectionalLight(color, intensity);
  light.position.set(-1, 2, 4);
  scene.add(light);
}

const cubes = [
  makeInstance(geometry, 0x44aa88,  0),
  makeInstance(geometry, 0x8844aa, -2),
  makeInstance(geometry, 0xaa8844,  2),
];

//Function to make an instance of cubes
function makeInstance(geometry, color, x) {
  const material = new THREE.MeshPhongMaterial({color});

  //Creating a mesh and giving it the geometry and material
  const cube = new THREE.Mesh(geometry, material);
  scene.add(cube);

  //default position is 0,0,0
  cube.position.x = x;

  return cube;
}

//Adding the scene and camera to the renderer
renderer.render(scene, camera);

//https://developer.mozilla.org/en-US/docs/Web/API/window/requestAnimationFrame
//Passing a callback function to the requestAnimationFrame
requestAnimationFrame(render);

//Callback function passed to requestAniationFrame
//The callback method is passed a single argument, a DOMHighResTimeStamp, which indicates the current time
function render(time) {
  time *= 0.001;  // convert time to seconds

  cubes.forEach((cube, ndx) => {
    const speed = 1 + ndx * .1;
    const rot = time * speed;
    cube.rotation.x = rot;
    cube.rotation.y = rot;
  });

  renderer.render(scene, camera); //Draws our scene

  //The callback routineis itself calling requestAnimationFrame() to animate another frame at the next repaint.
  requestAnimationFrame(render);
}
