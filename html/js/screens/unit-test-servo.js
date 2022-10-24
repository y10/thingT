import { jQuery } from "../system/jquery";
import { Http } from "../system";

const fingers = ["Thumb", "Index", "Middle", "Ring", "Little"];

const html = (index)=> `
<div class="container">
    <h1 ID=>${fingers[index]}</h1>
    <section>
        <button id="test0">0°</button>
        <button id="test90">90°</button>
        <button id="test180">180°</button>
    </section>
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
  
button {
  border: 0;
  border-radius: 0.3rem;
  background-color: var(--secondary-background-color);
  line-height: 2.4rem;
  font-size: 1.2rem;
  width: 100%;
  color: var(--secondary-text-color);
  margin: 8px 0;
}

</style>
`;
export class UnitTestServo extends HTMLElement {
  connectedCallback() {
    const index = this.index = this.getAttribute("index");
    this.jQuery = jQuery(this).attachShadowTemplate(style + html(parseInt(index)), async ($) => {
      this.btnTest0 = $("#test0");
      this.btnTest0.on("click", (e) => this.onTest(0));
      this.btnTest90 = $("#test90");
      this.btnTest90.on("click", (e) => this.onTest(90));
      this.btnTest180 = $("#test180");
      this.btnTest180.on("click", (e) => this.onTest(180));
    });
  }

  async onTest(degree) {
    await Http.json("GET", `api/servo/${this.index}/position?v=${degree}`);
  }

  disconnectedCallback() {
    this.jQuery().detach();
  }
}
