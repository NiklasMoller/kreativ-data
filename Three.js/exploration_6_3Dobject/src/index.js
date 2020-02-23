import * as THREE from 'three';
import { GLTFLoader } from 'three/examples/jsm/loaders/GLTFLoader';
import { OrbitControls } from 'three/examples/jsm/controls/OrbitControls.js';

import * as dat from 'dat.gui';
const gui = new dat.GUI();


var renderer = new THREE.WebGLRenderer();
renderer.setSize( window.innerWidth, window.innerHeight );
renderer.outputEncoding = THREE.sRGBEncoding; //Configure the encoding to load GLTF data
document.body.appendChild( renderer.domElement );

const fov = 75; //Field of view
const aspect = 2;  // the canvas default
const near = 0.1; //Closer than this wont be shown
const far = 1000; //Further than this wont be shown
const camera = new THREE.PerspectiveCamera(fov, aspect, near, far); //This defines the "frustum" which can be seen as a pyramid with a tip sliced off

camera.position.z = -20;

const scene = new THREE.Scene();





var controls = new OrbitControls( camera, renderer.domElement );
//controls.update() must be called after any manual changes to the camera's transform
camera.position.set( 0, 20, 100 );
controls.update();

//Helper class, pass a color from Three.js and convert it to HEX color string
class ColorGUIHelper {
  constructor(object, prop) {
    this.object = object;
    this.prop = prop;
  }
  get value() {
    return `#${this.object[this.prop].getHexString()}`;
  }
  set value(hexString) {
    this.object[this.prop].set(hexString);
  }
}

const color = 0xFFFFFF;
const intensity = 1;
const light = new THREE.AmbientLight(color, intensity);
scene.add(light);


//gui.addFolder(name)
// Adds a folder with specified name
const folder_AmbientLight = gui.addFolder('AmbiendLight');

//gui.add(object, property, [min], [max], [step])
//Adds Object to be manipulated, its proprty name, min, max and increment value
folder_AmbientLight.add(light, 'intensity', 0, 2, 0.1); //Lower bound, Upper Bound

//gui.addColor(object, property)
//Adds Object to be manipulated, its property name
folder_AmbientLight.addColor(new ColorGUIHelper(light, 'color'), 'value').name('color');




//const helper = new THREE.AmbientLightHelper(pointLight, 1);
//scene.add(helper);

/*
function makeXYZGUI(gui, vector3, name, onChangeFn) {
  const folder = gui.addFolder(name);
  folder.add(vector3, 'x', -10, 10).onChange(onChangeFn);
  folder.add(vector3, 'y', 0, 10).onChange(onChangeFn);
  folder.add(vector3, 'z', -10, 10).onChange(onChangeFn);
  folder.open();
}

function updateLight{
  helper.update();
}

updateLight();




makeXYZGUI(gui, pointLight.position, 'position', updateLight);
*/


//Adding the scene and camera to the renderer
renderer.render(scene, camera);

//-------- MATERIALS ETC GOES BELOW -------


/*
var geometry = new THREE.BoxGeometry();
var material = new THREE.MeshBasicMaterial( { color: 0x00ff00 } );
var cube = new THREE.Mesh( geometry, material );
scene.add( cube );


*/

var loader = new GLTFLoader();

loader.load( 'scene.gltf', function ( gltf ) {
	scene.add( gltf.scene );
}, undefined, function ( error ) {
	console.error( error );
  console.log("Could not load the 3D object");
} );




// ------- RENDER THE SCENE AND CAMERA --------
//https://developer.mozilla.org/en-US/docs/Web/API/window/requestAnimationFrame
//Passing a callback function to the requestAnimationFrame
requestAnimationFrame(render);


//Callback function passed to requestAniationFrame
//The callback method is passed a single argument, a DOMHighResTimeStamp, which indicates the current time
function render(time) {
  time *= 0.001;  // convert time to seconds


  //cube.rotation.x += 0.01;
//cube.rotation.y += 0.01;


  renderer.render(scene, camera); //Draws our scene

  //The callback routineis itself calling requestAnimationFrame() to animate another frame at the next repaint.
  requestAnimationFrame(render);
}
