import * as THREE from 'three';
import {gui, ColorGUIHelper } from './assets/js/guiImporter.js';
import { OrbitControls } from 'three/examples/jsm/controls/OrbitControls.js';

var renderer = new THREE.WebGLRenderer();
renderer.setSize( window.innerWidth, window.innerHeight );
document.body.appendChild( renderer.domElement );

const fov = 75; //Field of view
const aspect = 2;  // the canvas default
const near = 0.1; //Closer than this wont be shown
const far = 1000; //Further than this wont be shown
const camera = new THREE.PerspectiveCamera(fov, aspect, near, far); //This defines the "frustum" which can be seen as a pyramid with a tip sliced off
camera.position.z = 2; //Moving the camera back from 0.0.0 to 0.0.2


var controls = new OrbitControls( camera, renderer.domElement );
controls.update(); //must be called after any manual changes to the camera's transform

const scene = new THREE.Scene();

//Adding the scene and camera to the renderer
renderer.render(scene, camera);

//-------- MATERIALS ETC GOES BELOW -------


var geometry = new THREE.BoxGeometry();
var material = new THREE.MeshBasicMaterial( { color: 0x00ff00 } );
var cube = new THREE.Mesh( geometry, material );
scene.add( cube );


// ------- RENDER THE SCENE AND CAMERA --------
//https://developer.mozilla.org/en-US/docs/Web/API/window/requestAnimationFrame
//Passing a callback function to the requestAnimationFrame
requestAnimationFrame(render);


//Callback function passed to requestAniationFrame
//The callback method is passed a single argument, a DOMHighResTimeStamp, which indicates the current time
function render(time) {
  time *= 0.001;  // convert time to seconds

  renderer.render(scene, camera); //Draws our scene

  //The callback routineis itself calling requestAnimationFrame() to animate another frame at the next repaint.
  requestAnimationFrame(render);
}
