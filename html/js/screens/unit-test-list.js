import { Router } from "../system/router";
import { jQuery } from "../system/jquery";

const template = `<sketch-slider>
  <unit-servo index=0></unit-servo>
  <unit-servo index=1></unit-servo>
  <unit-servo index=2></unit-servo>
  <unit-servo index=3></unit-servo>
  <unit-servo index=4></unit-servo>
</sketch-slider>`;
export class UnitTestList extends HTMLElement {
  connectedCallback() {
    this.jQuery = jQuery(this).attachShadowTemplate(template, ($) => {
    });
  }

  disconnectedCallback() {
    this.jQuery().detach();
  }
}
