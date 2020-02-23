import * as dat from 'dat.gui';
const gui = new dat.GUI();

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

export { gui, ColorGUIHelper };
