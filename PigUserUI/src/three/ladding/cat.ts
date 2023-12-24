import { Group } from 'three';
import * as three from 'three';
import gsap from 'gsap';
import { GLTF } from 'three/addons/loaders/GLTFLoader.js';

import catAsset from '@/assets/model/cat_rigged.glb?url';
import { addDebug, loadGLTF } from '../common';

export class Cat extends Group {
  private _mixer: three.AnimationMixer;
  private _model: GLTF;
  private _mtxTS: three.Matrix4;
  private _light: three.PointLight;

  constructor() {
    super();
  }

  async init() {
    // Light init
    this._light = new three.PointLight( 0xffffff, 10, 50 );
    this._light.castShadow = true;
    this._light.shadow.mapSize.width = 512;
    this._light.shadow.mapSize.height = 512;
    this._light.shadow.camera.near = 0.1;
    this._light.shadow.camera.far = 200;
    this.add(this._light);

    // Cat base transform
    const mtxTemp = new three.Matrix4();
    this._mtxTS = new three.Matrix4();
    this._mtxTS.multiply(mtxTemp.makeTranslation(0, -0.2, 7));
    this._mtxTS.multiply(mtxTemp.makeScale(2.5, 2.5, 2.5));

    // Cat load model
    this._model = await loadGLTF(catAsset);
    const scene = this._model.scene;
    const sceneMatrix = scene.matrix;
    scene.matrixAutoUpdate = false;

    const sceneRot = scene.rotation.clone();
    gsap.to(sceneRot, { y: Math.PI * 2, duration: 25, repeat: 1111, ease: "sine", onUpdate: () => {
      sceneMatrix.copy(mtxTemp.makeRotationY(sceneRot.y));
      sceneMatrix.multiply(this._mtxTS);
      
      scene.getWorldPosition(this._light.position);
      this._light.position.y = 0.1;
    }});

    this.add(scene);
    this._mixer = new three.AnimationMixer(scene);
    this._mixer.clipAction(this._model.animations[0]).play();

    // Debug
    if (import.meta.env.DEV) {
      const pointLightHelper = new three.PointLightHelper(this._light, 0.15);
      this.add(pointLightHelper);
      addDebug(pointLightHelper);
    }
  }

  update(delta: number) {
    this._mixer.update(delta);
  }
}