//Three.js code goes here
//Tutorial from https://threejsfundamentals.org/threejs/lessons/threejs-scenegraph.html

var renderer = new THREE.WebGLRenderer();
renderer.setSize( window.innerWidth, window.innerHeight );
document.body.appendChild( renderer.domElement );


// an array of objects whose rotation to update
const objects = [];

const scene = new THREE.Scene();

//Simple point light
{
  const color = 0xFFFFFF;
  const intensity = 3;
  const light = new THREE.PointLight(color, intensity);
  scene.add(light);
}

const fov = 40;
const aspect = 2;  // the canvas default
const near = 0.1;
const far = 1000;
const camera = new THREE.PerspectiveCamera(fov, aspect, near, far);
camera.position.set(0, 50, 0);
camera.up.set(0, 0, 1);
camera.lookAt(0, 0, 0);

// Creating a Sphere geometry with its properties
const radius = 1;
const widthSegments = 6;
const heightSegments = 6;
const sphereGeometry = new THREE.SphereBufferGeometry(
    radius, widthSegments, heightSegments);

//Empy object node
const solarSystem = new THREE.Object3D();
scene.add(solarSystem);
objects.push(solarSystem); //Pushing this empty object into the objects makes it rotate on y axis

const sunMaterial = new THREE.MeshPhongMaterial({emissive: 0xFFFF00}); //Material for the sun
const sunMesh = new THREE.Mesh(sphereGeometry, sunMaterial);//Mesh for the sun
sunMesh.scale.set(5, 5, 5); //Making the sun large
solarSystem.add(sunMesh); //Adding this mesh to the SolarsystemNode
objects.push(sunMesh);//pushing the mesh into the array of objects. Since it is a part of solarsystem, it will rotate twice as much

const earthOrbit = new THREE.Object3D();
earthOrbit.position.x = 10;
solarSystem.add(earthOrbit);
objects.push(earthOrbit); //Same here as above. It is rotating as a part of the solar system and as a independent object

//This makes the earth rotate both as a part of solar system, as a part of earth orbit and as an independent object
const earthMaterial = new THREE.MeshPhongMaterial({color: 0x2233FF, emissive: 0x112244}); //Emessive color unaffected by light
const earthMesh = new THREE.Mesh(sphereGeometry, earthMaterial);
earthOrbit.add(earthMesh); //Adding mesh to earth orbit
objects.push(earthMesh); //Makes it rotate

//Same as above
const moonOrbit = new THREE.Object3D();
moonOrbit.position.x = 2;
earthOrbit.add(moonOrbit);

const moonMaterial = new THREE.MeshPhongMaterial({color: 0x888888, emissive: 0x222222});
const moonMesh = new THREE.Mesh(sphereGeometry, moonMaterial);
moonMesh.scale.set(.5, .5, .5);
moonOrbit.add(moonMesh);
objects.push(moonMesh);

//Adding the scene and camera to the renderer
renderer.render(scene, camera);

//https://developer.mozilla.org/en-US/docs/Web/API/window/requestAnimationFrame
//Passing a callback function to the requestAnimationFrame
requestAnimationFrame(render);

if (resizeRendererToDisplaySize(renderer)) {
  const canvas = renderer.domElement;
  camera.aspect = canvas.clientWidth / canvas.clientHeight;
  camera.updateProjectionMatrix();
}

//Callback function passed to requestAniationFrame
//The callback method is passed a single argument, a DOMHighResTimeStamp, which indicates the current time
function render(time) {
  time *= 0.001;  // convert time to seconds

  objects.forEach((obj) => {
    obj.rotation.y = time;
  });

  renderer.render(scene, camera); //Draws our scene

  //The callback routineis itself calling requestAnimationFrame() to animate another frame at the next repaint.
  requestAnimationFrame(render);
}


function resizeRendererToDisplaySize(renderer) {
  const canvas = renderer.domElement;
  const width = canvas.clientWidth;
  const height = canvas.clientHeight;
  const needResize = canvas.width !== width || canvas.height !== height;
  if (needResize) {
    renderer.setSize(width, height, false);
  }
  return needResize;
}
