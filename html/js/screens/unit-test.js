import { jQuery } from "../system/jquery";
import { Http } from "../system";

const fingers = ['thumb', 'index', 'middle', 'ring', 'little'];

const html = `
<div class="container">
    <h1>Servos</h1>
    <form>
        <p>Thumb</p>
        <input type="range" min="0" max="180" value="90" class="slider" id="thumb">
        <p>Index</p>
        <input type="range" min="0" max="180" value="90" class="slider" id="index">
        <p>Middle</p>
        <input type="range" min="0" max="180" value="90" class="slider" id="middle">
        <p>Ring</p>
        <input type="range" min="0" max="180" value="90" class="slider" id="ring">
        <p>Little</p>
        <input type="range" min="0" max="180" value="90" class="slider" id="little">
    </form>
</div>
`;
const style = `
<style>

.container {
  width: 80vw;
  max-width:300px;
}

h1 {
  position: absolute;
  top: 0;
}
  @media screen and (min-height: 730px) {
    h1 { top: 6%; }
  }

p {
  margin-block-start: 1em;
  margin-block-end: 0px
}
  
input {
    padding: 8px;
    font-size: 1em;
    width: 100%;
}

</style>
`;
export class UnitTest extends HTMLElement {
  connectedCallback() {
    this.jQuery = jQuery(this).attachShadowTemplate(style + html, async ($) => {
      this.sldThumb = $("#thumb");
      this.sldThumb.on("change", (e) =>
        this.onServoChange({ target: this.sldThumb.item(), index: 0 })
      );
      this.sldIndex = $("#index");
      this.sldIndex.on("change", (e) =>
        this.onServoChange({ target: this.sldIndex.item(), index: 1 })
      );
      this.sldMiddle = $("#middle");
      this.sldMiddle.on("change", (e) =>
        this.onServoChange({ target: this.sldMiddle.item(), index: 2 })
      );
      this.sldRing = $("#ring");
      this.sldRing.on("change", (e) =>
        this.onServoChange({ target: this.sldRing.item(), index: 3 })
      );
      this.sldLittle = $("#little");
      this.sldLittle.on("change", (e) =>
        this.onServoChange({ target: this.sldLittle.item(), index: 4 })
      );

      try {
        await this.load();
      } catch (error) {
        console.error(error);
      }
    });
  }

  async load() {
    const result = await Promise.all([
      Http.json("GET", `api/servo/0/state`),
      Http.json("GET", `api/servo/1/state`),
      Http.json("GET", `api/servo/2/state`),
      Http.json("GET", `api/servo/3/state`),
      Http.json("GET", `api/servo/4/state`),
    ]);

    const {servos} = result.reduce(
      (r, c) => {
        r.servos = { ...r.servos, ...c.servos };
        return r;
      },
      { servos: {} }
    );

    for (const key in servos) {
      this.jQuery("#" + fingers[key]).value(servos[key]);
    }
  }

  async onServoChange(e) {
    const { index, target } = e;
    const { value: input } = target;
    const { servos } = await Http.json(
      "GET",
      `api/servo/${index}/position?v=${input}`
    );
    const output = servos[index];
    target.value = output;
  }

  disconnectedCallback() {
    this.jQuery().detach();
  }
}
