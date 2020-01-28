//Simple code to load images as textures. Using the LoadManager and a progress bar

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

//Adding the scene and camera to the renderer
renderer.render(scene, camera);

//-------- MATERIALS ETC GOES BELOW -------

//Creating a geometry
const boxWidth = 1;
const boxHeight = 1;
const boxDepth = 1;
const geometry = new THREE.BoxGeometry(boxWidth, boxHeight, boxDepth);

const loadManager = new THREE.LoadingManager(); //Load manager, to allow website to wait for load
const loader = new THREE.TextureLoader(loadManager); //The loader to load images into three.js. Passing in the LoadManager

const material = new THREE.MeshBasicMaterial({
  color: 0xFF8844,
  map: loader.load('sci-fi.jpg'),
});

const sciFiCube = new THREE.Mesh(geometry, material);
sciFiCube.position.x = 2;
//scene.add(sciFiCube);


const materials = [
  new THREE.MeshBasicMaterial({map: loader.load('flower-1.jpg')}),
  new THREE.MeshBasicMaterial({map: loader.load('flower-2.jpg')}),
  new THREE.MeshBasicMaterial({map: loader.load('flower-3.jpg')}),
  new THREE.MeshBasicMaterial({map: loader.load('flower-4.jpg')}),
  new THREE.MeshBasicMaterial({map: loader.load('flower-5.jpg')}),
  new THREE.MeshBasicMaterial({map: loader.load('flower-6.jpg')}),
];

const flowerCube = new THREE.Mesh(geometry, materials);
//scene.add(flowerCube);

//Root node to hold the cubes
const cubes = new THREE.Object3D();

//For the progess loader
const loadingElem = document.querySelector('#loading');
const progressBarElem = loadingElem.querySelector('.progressbar');

//Loading the two cubes into a root node using the LoadManager
loadManager.onLoad = () => {
   loadingElem.style.display = 'none';
  cubes.add(flowerCube); //Adding cubes to a root object
  cubes.add(sciFiCube);
  scene.add(cubes);
};

loadManager.onProgress = (urlOfLastItemLoaded, itemsLoaded, itemsTotal) => {
  const progress = itemsLoaded / itemsTotal;
  progressBarElem.style.transform = `scaleX(${progress})`;
};

// ------- RENDER THE SCENE AND CAMERA --------
//https://developer.mozilla.org/en-US/docs/Web/API/window/requestAnimationFrame
//Passing a callback function to the requestAnimationFrame
requestAnimationFrame(render);
//Callback function passed to requestAniationFrame
//The callback method is passed a single argument, a DOMHighResTimeStamp, which indicates the current time
function render(time) {
  time *= 0.001;  // convert time to seconds

  cubes.rotation.x = time; //rotating the root object
  cubes.rotation.y = time;

  renderer.render(scene, camera); //Draws our scene

  //The callback routineis itself calling requestAnimationFrame() to animate another frame at the next repaint.
  requestAnimationFrame(render);
}
