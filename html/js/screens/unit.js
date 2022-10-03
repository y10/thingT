import { Router } from "../system/router";
import { jQuery } from "../system/jquery";

const template = `<unit-servos></unit-servos>`;
export class UnitControl extends HTMLElement {
  connectedCallback() {
    this.jQuery = jQuery(this).attachShadowTemplate(template, ($) => {
      $(document).on("back", this.onBack.bind(this));
      $(document).on("slide-y", this.onSlide.bind(this));
    });
  }

  disconnectedCallback() {
    this.jQuery().detach();
  }

  onSlide(e) {
    if (e.up && this.jQuery(this).isAttached()) {
      if (Router.navigate("menu", false)) {
        this.visibleMenu = true;
      }
    } else if (e.down && this.visibleMenu) {
      Router.refresh();
    }
  }

  onBack(e) {
    this.jQuery().html(template);
  }
}
