import gsap from 'gsap';
import ScrollTrigger from 'gsap/ScrollTrigger';
import * as three from 'three';
import { WaterSimulation } from '../water/simulation';
import { Cat } from './cat';
import { addDebug } from '../common';

import abc from '@/assets/texture/abc.jpg';

export class AnimationRoundHandler {
  private _renderer: three.WebGLRenderer;
  private _scene: three.Scene;
  private _camera: three.Camera;
  private _cube: three.Group;
  private _water: WaterSimulation;
  private _cat: Cat;
  private _isRunning = true;

  private _clock = new three.Clock();
  private _delta = 0;
  private _interval = 1 / 60;

  constructor(private readonly container: HTMLElement) {
    const width = window.innerWidth, height = window.innerHeight;
    this._camera = new three.PerspectiveCamera( 70, width / height, 0.01, 1000 );
    this._scene = new three.Scene();
    this._scene.fog = new three.Fog(0xcccccc, 20, 50);

    const light = new three.AmbientLight(0xffffff, 0.01);
    this._scene.add(light);
  
    this._cube = this.buildRound();
    this._scene.add(this._cube);
    this._camera.position.z = 10;
    this._camera.position.x = 2;
    this._camera.position.y = 2;

    this._renderer = new three.WebGLRenderer( { antialias: true } );
    this._renderer.setSize( width, height );
    this._renderer.setClearColor( 0x000000, 0 );
    this._renderer.shadowMap.enabled = true;
    this._renderer.shadowMap.type = three.PCFSoftShadowMap;
    container.appendChild(this._renderer.domElement);

    if (import.meta.env.DEV) {
      const gridHelper = new three.GridHelper(30, 20);
      const axesHelper = new three.AxesHelper(10);
      this._scene.add(gridHelper);
      this._scene.add(axesHelper);
      addDebug(gridHelper, axesHelper);
    }
    
    this._water = new WaterSimulation();
    this._cat = new Cat();
    this._scene.add(this._water);
    this._scene.add(this._cat);

    this.init().then(() => {
      this.onAnimation();
    })
  }

  async init() {
    await this._water.init();
    await this._cat.init();
  }

  initGsap(id: string) {
    gsap.registerPlugin(ScrollTrigger);
    ScrollTrigger.defaults({
      immediateRender: false,
      scrub: true,
    });
    const tl = gsap.timeline();
    tl.to(this._cube.rotation, { y: 1, scrollTrigger: {
      trigger: id,
      start: 'top bottom',
      end: 'top top',
    }});
  }

  release() {
    this._isRunning = false;
    this.container.removeChild(this._renderer.domElement);
  }

  onAnimation = () => {
    if (this._isRunning) {
      requestAnimationFrame(this.onAnimation);
    }

    const d = this._clock.getDelta();
    this._delta += d;
    this._water.update(d);
    this._cat.update(d);
    if (this._delta  > this._interval) {
      this._water.render();
      this.render();
      this._delta = this._delta % this._interval;
    }
  }

  private render() {
	  this._renderer.render(this._scene, this._camera);
  }

  private buildRound() {
    const texture = new three.TextureLoader().load(abc);
    texture.wrapS = three.RepeatWrapping;
    texture.wrapT = three.RepeatWrapping;

    const group = new three.Group();
    const mxTemp = new three.Matrix4();
    const mx = new three.Matrix4();
    for (let i = 0; i <= 360; i+= 30) {
      mx.identity();
      mx.multiply(mxTemp.makeRotationY(i * Math.PI / 180));
      mx.multiply(mxTemp.makeTranslation(3, 0, 0));
      const geometry = new three.BoxGeometry(0.1, 1.6, 1.4);
      const material = new three.MeshPhongMaterial( { map: texture } );
      const cube = new three.Mesh(geometry, material);
      cube.castShadow = true;
      cube.receiveShadow = true;
      cube.matrix.copy(mx);
      cube.matrixAutoUpdate = false;
      group.add(cube);
    }

    group.position.set(0, 0.3, 1);
    return group;
  }
}