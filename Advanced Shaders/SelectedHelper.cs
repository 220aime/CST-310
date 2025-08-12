using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using UnityEngine;
using UnityEngine.Events;
using UnityEngine.EventSystems;
using UnityEngine.UI;

namespace HMI.UI.IVI
{
    /// <summary>
    /// Activates/deactivates sets of objects based on selection,
    /// and (optionally) drives a Shader Graph float property (e.g. _Speed)
    /// on target renderers.
    /// </summary>
    public class SelectedHelper : MonoBehaviour, ISelectHandler, IDeselectHandler
    {
        // ---------------- Existing fields ----------------
        public List<GameObject> SelectSet = new List<GameObject>();
        public List<GameObject> DeselectSet = new List<GameObject>();
        public UnityEvent AddtionalActionOnToggleIsOn;
        public UnityEvent Selected;
        public UnityEvent Deselected;
        public bool IsSelected { get; private set; } = false;

        // ---------------- New: shader speed control ----------------
        [Header("Shader Speed Control (optional)")]
        [Tooltip("Renderers that use your Shader Graph material (with a float property called _Speed).")]
        public List<Renderer> TargetRenderers = new List<Renderer>();

        [Tooltip("Shader Graph float property reference name.")]
        public string speedPropertyName = "_Speed";

        [Tooltip("Speed to apply when this control is selected.")]
        public float speedOnSelect = 2f;

        [Tooltip("Speed to apply when this control is deselected (0 = stop).")]
        public float speedOnDeselect = 0f;

        [Tooltip("Speed when toggle is ON/OFF (used by ApplyToggleSpeed).")]
        public float toggleOnSpeed = 2f;
        public float toggleOffSpeed = 0f;

        // current speed cache (useful for nudging, etc.)
        [SerializeField, Tooltip("Current speed value written to the shader.")]
        private float currentSpeed = 0f;

        MaterialPropertyBlock _mpb;
        int _speedID;

        // ---------------- Unity lifecycle ----------------
        private void Awake()
        {
            _mpb = new MaterialPropertyBlock();
            _speedID = Shader.PropertyToID(speedPropertyName);
            Deselect(); // your original behavior
        }

        private void OnEnable()
        {
            if (EventSystem.current != null)
            {
                if (EventSystem.current.currentSelectedGameObject != gameObject)
                {
                    Deselect();
                }
                else
                {
                    Select();
                }
            }
        }

        private void OnValidate()
        {
            // Keep property ID up-to-date when edited in Inspector
            _speedID = Shader.PropertyToID(speedPropertyName);
            // Apply current speed in edit mode so you see changes immediately
            if (_mpb == null) _mpb = new MaterialPropertyBlock();
            ApplySpeedToAll(currentSpeed);
        }

        // ---------------- EventSystem callbacks ----------------
        public void OnSelect(BaseEventData data) => Select();
        public void OnDeselect(BaseEventData data) => Deselect();

        // ---------------- Selection logic ----------------
        private void Select()
        {
            IsSelected = true;
            SetState(SelectSet, true);
            SetState(DeselectSet, false);
            Selected.Invoke();

            // NEW: write speed when selected
            SetSpeed(speedOnSelect);
        }

        private void Deselect()
        {
            IsSelected = false;
            SetState(SelectSet, false);
            SetState(DeselectSet, true);
            Deselected.Invoke();

            // NEW: write speed when deselected
            SetSpeed(speedOnDeselect);
        }

        private void SetState(IEnumerable<GameObject> set, bool state)
        {
            foreach (var obj in set)
            {
                if (obj) obj.SetActive(state);
            }
        }

        // ---------------- Public hooks for your UI ----------------

        /// <summary>Sets the shader speed on all TargetRenderers.</summary>
        public void SetSpeed(float value)
        {
            currentSpeed = value;
            ApplySpeedToAll(currentSpeed);
        }

        /// <summary>For Slider.onValueChanged (drag the component, pick this method).</summary>
        public void SetSpeedFromSlider(Slider slider)
        {
            if (slider != null) SetSpeed(slider.value);
        }

        /// <summary>For Toggle.onValueChanged (drag the component, pick this method).</summary>
        public void ApplyToggleSpeed(Toggle t)
        {
            if (t != null) SetSpeed(t.isOn ? toggleOnSpeed : toggleOffSpeed);
        }

        /// <summary>For Buttons or other events that pass a float argument.</summary>
        public void SetSpeedTo(float value) => SetSpeed(value);

        /// <summary>Nudge by delta (e.g., call from keybinds or buttons).</summary>
        public void NudgeSpeed(float delta) => SetSpeed(currentSpeed + delta);

        // ---------------- Internal helper ----------------
        private void ApplySpeedToAll(float value)
        {
            if (TargetRenderers == null) return;

            foreach (var r in TargetRenderers)
            {
                if (!r) continue;

                // Use MaterialPropertyBlock so we don't duplicate materials
                r.GetPropertyBlock(_mpb);
                _mpb.SetFloat(_speedID, value);
                r.SetPropertyBlock(_mpb);
            }
        }

        // ---------------- Your existing toggle helper ----------------
        public void AddtionActionsOnToggle(Toggle ToggleComponent)
        {
            if (ToggleComponent != null && ToggleComponent.isOn)
            {
                AddtionalActionOnToggleIsOn.Invoke();
            }
        }
    }
}
