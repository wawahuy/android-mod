import { Group } from 'three';
import * as three from 'three';
import land from '@/assets/texture/land.png';

export class Terrian extends Group {

  constructor() {
    super();
  }

  async init() {
    const texture = new three.TextureLoader().load(land);
    texture.wrapS = three.RepeatWrapping;
    texture.wrapT = three.RepeatWrapping;
    texture.repeat.set(20, 20);
    const geometry = new three.BoxGeometry(30, 1, 30);
    const material = new three.MeshPhongMaterial( { map: texture } );
    const cube = new three.Mesh(geometry, material);
    cube.receiveShadow = true;
    cube.position.set(0, -1, 0);
    this.add(cube);
  }
}