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

    // generation attr position
    const geometry = new three.BufferGeometry();

    const planeSize = 1000;
    const segments = 1;
    const vertices: number[] = [];
    const uvs = [];
    for (let i = 0; i <= segments; i++) {
      for (let j = 0; j <= segments; j++) {
        const x = (i / segments) * planeSize - planeSize / 2;
        const z = (j / segments) * planeSize - planeSize / 2;
        const y = 0;
        vertices.push(x, y, z);

        const u = i / segments;
        const v = j / segments;
        uvs.push(u, v);
      }
    }
    const verticesArray = new Float32Array(vertices);
    const uvsArray = new Float32Array(uvs);
    geometry.setAttribute('position', new three.Float32BufferAttribute(verticesArray, 3));
    geometry.setAttribute('uv', new three.Float32BufferAttribute(uvsArray, 2));

    const indices = [];
    for (let i = 0; i < segments; i++) {
        for (let j = 0; j < segments; j++) {
            const a = i * (segments + 1) + j;
            const b = (i + 1) * (segments + 1) + j;
            const c = i * (segments + 1) + j + 1;
            const d = (i + 1) * (segments + 1) + j + 1;

            // Face 1
            indices.push(a, b, d);

            // Face 2
            indices.push(a, d, c);
        }
    }
    console.log(indices.length)
    const indicesArray = new Uint32Array(indices);
    geometry.setIndex(new three.Uint32BufferAttribute(indicesArray, 1));
    geometry.computeVertexNormals();

    // mesh
    const material = new three.MeshPhongMaterial( { map: texture, side: three.BackSide } );
    const cube = new three.Mesh(geometry, material);
    cube.receiveShadow = true;
    cube.position.set(0, 0, 0);
    this.add(cube);
  }


}