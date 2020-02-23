//Based on https://raw.githubusercontent.com/mrdoob/three.js/master/examples/misc_controls_deviceorientation.html
//Picture from https://www.flickr.com/photos/jonragnarsson/2294472375/in/photostream/

var startButton = document.getElementById('startButton');

startButton.addEventListener('click', function () {

				init();
				animate();

			}, false );


function init (){

  var overlay = document.getElementById( 'overlay' );
  overlay.remove();

  const fov = 75; //Field of view
  const aspect = window.innerWidth / window.innerHeight;
  const near = 1; //Closer than this wont be shown
  const far = 1100; //Further than this wont be shown
  const camera = new THREE.PerspectiveCamera(fov, aspect, near, far); //This defines the "frustum" which can be seen as a pyramid with a tip sliced off

  const controls = new THREE.DeviceOrientationControls();


  const scene = new THREE.Scene();

  var geometry = new THREE.SphereBufferGeometry( 500, 60, 40 );
  // invert the geometry on the x-axis so that all of the faces point inward
  geometry.scale( - 1, 1, 1 );


  var material = new THREE.MeshBasicMaterial( {
  map: new THREE.TextureLoader().load( 'interior.jpg' )
} );

  var mesh = new THREE.Mesh( geometry, material );
  scene.add( mesh );

  renderer = new THREE.WebGLRenderer( { antialias: true } );
renderer.setPixelRatio( window.devicePixelRatio );
renderer.setSize( window.innerWidth, window.innerHeight );
document.body.appendChild( renderer.domElement );

//

window.addEventListener( 'resize', onWindowResize, false );


}

//camera.position.z = 2; //Moving the camera back from 0.0.0 to 0.0.2



function animate() {

  window.requestAnimationFrame( animate );

  controls.update();
  renderer.render( scene, camera );

}

function onWindowResize() {

  camera.aspect = window.innerWidth / window.innerHeight;
  camera.updateProjectionMatrix();

  renderer.setSize( window.innerWidth, window.innerHeight );

}
